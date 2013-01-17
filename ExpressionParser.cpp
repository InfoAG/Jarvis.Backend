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
        for (const auto &mod : modpkg->getModules().terminalExpressions) {
            qDebug() << "\t\t" << mod->name() << "\t" << mod->description() << "\t";
        }
        qDebug() << "\tBinaryOperators:";
        for (const auto &mod : modpkg->getModules().binaryOperators) {
            qDebug() << "\t\t" << mod->name() << "\t" << mod->description() << "\t";
        }
        qDebug() << "\tUnaryOperators:";
        for (const auto &mod : modpkg->getModules().unaryOperators) {
            qDebug() << "\t\t" << mod->name() << "\t" << mod->description() << "\t";
        }
        qDebug() << "\tFunctions:";
        for (const auto &mod : modpkg->getModules().functions) {
            qDebug() << "\t\t" << mod->name() << "\t" << mod->description() << "\t";
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

CAS::AbstractStatement::StatementP ExpressionParser::parseStatement(std::string input) const
{
    input = trim(input);
    /*bool deleted;
    do {
        input = trim(input);
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
    } while (deleted);*/

    CAS::AbstractStatement::StatementP result;
    for (const auto &terminal : modules.terminalStatements) {
        try {
            result = terminal->parse(input, std::bind(&ExpressionParser::parseStatement, this, std::placeholders::_1), std::bind(&ExpressionParser::parseExpression, this, std::placeholders::_1));
            if (result) return result;
        } catch (ParserException &) {}
    }

    return parseExpression(std::move(input));
}


CAS::AbstractExpression::ExpressionP ExpressionParser::parseExpression(std::string input) const
{
    input = trim(input);
    CAS::AbstractExpression::ExpressionP result;
    for (const auto &terminal : modules.terminalExpressions) {
        try {
            result = terminal->parse(input, std::bind(&ExpressionParser::parseExpression, this, std::placeholders::_1));
            if (result) return result;
        } catch (ParserException &) {}
    }

    auto level = 0;
    std::string::const_iterator foundPos = input.cend();
    std::pair<const BinaryOperatorModule*, size_t> bestBinOpMatch{nullptr, 0};
    CAS::AbstractExpression::ExpressionP parseForMatchResult;

    for (auto i = input.cbegin(); i != input.cend(); ++i) {
        if (*i == '(' || *i == '[' || *i == '{')  level--;
        else if (*i == ')' || *i == ']' || *i == '}') level++;
        else if (level == 0) {
            for (const auto &it_op : modules.binaryOperators) {
                auto candidate = it_op->matches(input, i - input.cbegin(), *this);
                if (candidate.first && (bestBinOpMatch.first == nullptr || it_op->priority() < bestBinOpMatch.first->priority() || (it_op->priority() == bestBinOpMatch.first->priority() && it_op->associativity() == BinaryOperatorInterface::LEFT))) {
                    if (! it_op->needsParseForMatch()) {
                        foundPos = i;
                        parseForMatchResult.reset();
                        bestBinOpMatch = {it_op.get(), candidate.second};
                    } else {
                        try {
                            auto tmpResult = it_op->parse(parseExpression(input.substr(0, i - input.begin())), parseExpression(input.substr(i - input.begin() + 1, input.length() - (i - input.begin()) - 1)));
                            if (tmpResult) {
                                bestBinOpMatch = {it_op.get(), candidate.second};
                                parseForMatchResult = std::move(tmpResult);
                            }
                        } catch (ParserException) {}
                    }
                }
            }
        }
    }

    std::pair<const UnaryOperatorModule*, size_t> bestUnOpMatch{nullptr, 0};
    for (const auto &it_op : modules.unaryOperators) {
        auto candidate = it_op->matches(input);
        if (candidate.first) {
            if (bestBinOpMatch.first == nullptr) {
                if (bestUnOpMatch.first == nullptr || it_op->priority() < bestUnOpMatch.first->priority())
                    bestUnOpMatch = {it_op.get(), candidate.second};
            } else if (it_op->priority() < bestBinOpMatch.first->priority())
                bestUnOpMatch = {it_op.get(), candidate.second};
        }
    }
    if (bestUnOpMatch.first != nullptr) {
        if (bestUnOpMatch.first->alignment() == UnaryOperatorInterface::PRE)
            return bestUnOpMatch.first->parse(parseExpression({input.cbegin() + bestUnOpMatch.second, input.cend()}));
        else
            return bestUnOpMatch.first->parse(parseExpression({input.cbegin(), input.cend() - bestUnOpMatch.second}));
    } else if (bestBinOpMatch.first != nullptr) {
        if (parseForMatchResult != nullptr) return parseForMatchResult;
        else return bestBinOpMatch.first->parse(parseExpression({input.cbegin(), foundPos}), parseExpression({foundPos + bestBinOpMatch.second, input.cend()}));
    }
if (input.back() != ')' || ! isalpha(input.front())) throw ParserException(std::move(input));
    std::string::iterator itParenthesis = std::find_if_not(input.begin() + 1, input.end(), isalnum);
    if (itParenthesis == input.begin() || itParenthesis == input.end() || *itParenthesis != '(') throw ParserException(std::move(input));
    foundPos = itParenthesis;
    std::string identifier{input.cbegin(), foundPos};
    //std::string argString = input.substr(foundPos + 1, input.length() - foundPos - 2);
    CAS::AbstractExpression::Expressions arguments;
    auto argString = tokenize({foundPos + 1, input.cend() - 1}, ",");
    for (const auto &arg : argString) arguments.emplace_back(parseExpression(arg));
    /*std::string::const_iterator lastPos = argString.begin();
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
    else arguments.emplace_back(parse(argString));*/

    const FunctionModule *best_func_match = nullptr;

    for (const auto &it_func : modules.functions) {
        if (it_func->matches(identifier, arguments.size()) && (best_func_match == nullptr || it_func->priority() > best_func_match->priority()))
            best_func_match = it_func.get();
    }
    if (best_func_match != nullptr)
        return best_func_match->parse(identifier, arguments);
    throw ParserException(std::move(input));
}

