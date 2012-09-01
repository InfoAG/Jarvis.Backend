#include "ExpressionParser.h"
#include <string>
#include <QDebug>

ExpressionParser::ExpressionParser(const QDir &module_dir)
{
    for (const auto &file : module_dir.entryInfoList(QStringList("*.jpkg"), QDir::Files)) {
        load(file.absoluteFilePath());
    }
    qDebug() << "Parser initialized" << endl;
    qDebug() << "Package Name\tModule Name\tModule Description";
    qDebug() << "";
    for (const auto &modpkg : modulePkgs) {
        qDebug() << modpkg->name();
        qDebug() << "\tTerminals:";
        for (const auto &mod : modpkg->getModules().terminals) {
            qDebug() << "\t\t" << mod.name() << "\t" << mod.description() << "\t";
        }
        qDebug() << "\tOperators:";
        for (const auto &mod : modpkg->getModules().operators) {
            qDebug() << "\t\t" << mod.name() << "\t" << mod.description() << "\t";
        }
        qDebug() << "\tFunctions:";
        for (const auto &mod : modpkg->getModules().functions) {
            qDebug() << "\t\t" << mod.name() << "\t" << mod.description() << "\t";
        }
    }
}

void ExpressionParser::unload(const QString &pkgName)
{
    auto pkg = std::find_if(modulePkgs.begin(), modulePkgs.end(), [&](const std::shared_ptr<ModulePackage> &it_pkg) {
            return it_pkg->name() == pkgName;
        });
    modules.removePkg(pkg->get());
    modulePkgs.erase(pkg);
}

std::shared_ptr<ModulePackage> ExpressionParser::load(const QString &filePath)
{
    auto tmp_module(std::make_shared<ModulePackage>(std::unique_ptr<QFile>(new QFile(filePath))));
    modulePkgs.append(tmp_module);
    modules += tmp_module->getModules();
    return tmp_module;
}

QVector<ModulePackage> ExpressionParser::getModulePkgs() const
{
    QVector<ModulePackage> result(modulePkgs.size());
    std::transform(modulePkgs.begin(), modulePkgs.end(), result.begin(), [](const std::shared_ptr<ModulePackage> &pkg) {
            return *pkg;
        });
    return result;
}

std::unique_ptr<CAS::AbstractArithmetic> ExpressionParser::parse(std::string input)
{
    int level;
    bool deleted;
    do {
        input.erase(std::remove_if(begin(input), end(input), isspace), end(input));
        if (input.empty()) throw "Error: Could not parse input.";
        deleted = false;
        if (input.at(0) == '(' && input.at(input.length() - 1) == ')') {
            level = 0;
            for (auto it = input.begin() + 1; it != input.end() - 1; ++it) {
                if (*it == '(') level++;
                else if (*it == ')' && --level == -1) break;
            }
            if (level != -1) {
                input.erase(0, 1);
                input.erase(input.length() - 1, 1);
                deleted = true;
            }
        }
    } while (deleted);

    std::unique_ptr<CAS::AbstractArithmetic> result;
    for (const auto &terminal : modules.terminals) {
        result = terminal.parse(input);
        if (result) return result;
    }

    level = 0;
    unsigned int foundPos = 0;
    const OperatorModule *best_op_match = nullptr;

    for (std::string::iterator i = input.begin(); i != input.end(); ++i) {
        if (*i == '(' || *i == '[' || *i == '{')  level--;
        else if (*i == ')' || *i == ']' || *i == '}') level++;
        else if (level == 0) {
            for (const auto &it_op : modules.operators) {
                if (it_op.matches(std::string(1, *i)) && (best_op_match == nullptr || it_op.priority() < best_op_match->priority() || (it_op.priority() == best_op_match->priority() && it_op.associativity() == OperatorInterface::LEFT))) {
                    best_op_match = &it_op;
                    foundPos = i - input.begin();
                }
            }
        }
    }
    //assignment operator matches for every '=', but parses only if first arg is variable / assignable function.
    //make sure we don't return nullptr in that case (is there a better way?)
    if (best_op_match != nullptr) {
        std::unique_ptr<CAS::AbstractArithmetic> result = best_op_match->parse(parse(input.substr(0, foundPos)), parse(input.substr(foundPos + 1, input.length() - foundPos - 1)));
        if (result) return result;
        else throw "Error: Could not parse input.";
    }
    if (input.back() != ')') throw "Error: Could not parse input.";
    std::string::iterator itParenthesis = std::find_if_not(input.begin(), input.end(), isalpha);
    if (itParenthesis == input.begin() || itParenthesis == input.end() || *itParenthesis != '(') throw "Error: Could not parse input.";
    foundPos = itParenthesis - input.begin();
    std::string identifier = input.substr(0, foundPos);
    std::string argString = input.substr(foundPos + 1, input.length() - foundPos - 2);
    std::vector<std::unique_ptr<CAS::AbstractArithmetic>> arguments;
    std::string::const_iterator lastPos = argString.begin();
    level = 0;
    for (std::string::const_iterator it = argString.begin(); it != argString.end(); ++it) {
        if (*it == '(' || *it == '[' || *it == '{')  level--;
        else if (*it == ')' || *it == ']' || *it == '}') level++;
        else if (level == 0 && *it == ',') {
            arguments.emplace_back(parse(argString.substr(lastPos - argString.begin(), it - lastPos)));
            lastPos = it + 1;
        }
    }
    if (lastPos != argString.begin()) arguments.emplace_back(parse(argString.substr(lastPos - argString.begin(), argString.length() - (lastPos - argString.begin()))));
    else arguments.emplace_back(parse(argString));

    const FunctionModule *best_func_match = nullptr;

    for (const auto &it_func : modules.functions) {
        if (it_func.matches(identifier, arguments.size()) && (best_func_match == nullptr || it_func.priority() > best_func_match->priority()))
            best_func_match = &it_func;
    }
    if (best_func_match != nullptr)
        return best_func_match->parse(identifier, arguments);
    throw "Error: Could not parse input.";
}
