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

    /**
     * Parse the input using all loaded packages
     * @param input Input string
     * @return Pointer to the result tree's root
     */
    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
