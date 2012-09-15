#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Arithmetic/AbstractArithmetic.h"
#include <QDir>
#include "ParserModules.h"
#include "ModulePackage.h"

/**
 * Parser with dynamic modules
 */
class ExpressionParser
{
private:
    ParserModules modules; //!< Parser modules
    QList<std::shared_ptr<ModulePackage> > modulePkgs; //!< Module packages
    std::string startLevelChars;
    std::string endLevelChars;

public:
    /**
     * Constructor
     * @param module_dir Directory to initialize modules from
     */
    ExpressionParser(const QDir &module_dir);
    /**
     * Unload a module package
     * @param pkgName Package name
     */
    void unload(const QString &pkgName);
    /**
     * Load a module package
     * @param pkgName Package name
     * @return Pointer to the loaded package
     */
    std::shared_ptr<ModulePackage> load(const QString &filePath);
    QVector<ModulePackage> getModulePkgs() const; //!< @return QVector of all module packages

    static std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters)
    {
        std::vector<std::string> tokens;
        int level = 0;
        auto lastPos = str.cbegin();
        for (auto it = str.cbegin(); it != str.cend(); ++it) {
            if (*it == '(' || *it == '[' || *it == '{')  level--;
            else if (*it == ')' || *it == ']' || *it == '}') level++;
            else if (level == 0 && delimiters.find_first_of(*it) != std::string::npos) {
                tokens.emplace_back(str.substr(lastPos - str.begin(), it - lastPos));
                lastPos = it + 1;
            }
        }
        if (lastPos != str.begin()) tokens.emplace_back(str.substr(lastPos - str.begin(), str.length() - (lastPos - str.begin())));
        else tokens.emplace_back(str);
        return tokens;
    }

    /**
     * Parse the input using all loaded packages
     * @param input Input string
     * @return Pointer to the result tree's root
     */
    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
