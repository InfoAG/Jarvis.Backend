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
    unsigned int op_pos = 0;
    const OperatorModule *best_op_match = nullptr;

    for (std::string::iterator i = input.begin(); i != input.end(); ++i) {
        if (*i == '(' || *i == '[' || *i == '{')  level--;
        else if (*i == ')' || *i == ']' || *i == '}') level++;
        else if (level == 0) {
            for (const auto &it_op : modules.operators) {
                if (it_op.matches(std::string(1, *i)) && (best_op_match == nullptr || it_op.priority() < best_op_match->priority() || (it_op.priority() == best_op_match->priority() && it_op.associativity() == OperatorInterface::LEFT))) {
                    best_op_match = &it_op;
                    op_pos = i - input.begin();
                }
            }
        }
    }
    if (best_op_match != nullptr)
        return best_op_match->parse(parse(input.substr(0, op_pos)), parse(input.substr(op_pos + 1, input.length() - op_pos - 1)));

    unsigned int pos_parenthesis = input.find_first_of('(');
    std::string identifier = input.substr(0, pos_parenthesis);
    for (const auto &it_func : modules.functions)
        if (it_func.matches(identifier)) return it_func.parse(parse(input.substr(pos_parenthesis + 1, input.length() - pos_parenthesis - 1)));
    throw "Error: Could not parse input.";
}
