#include "BinaryOperatorInterface.h"
#include "FunctionInterface.h"
#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Variable.h"
#include "Arithmetic/Assignment.h"
#include "Arithmetic/Function.h"
#include "Arithmetic/LazyEval.h"
#include <memory>
#include "variables_global.h"
#include "VariableDeclarationExpression.h"
#include "ExpressionParser.h"
#include "FunctionDeclarationExpression.h"
#include "FunctionDefinitionExpression.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT Variable_jmodule(const std::string &candidate)
{
    if (candidate.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
        return make_unique<CAS::Variable>(candidate);
    else return nullptr;
}

FunctionInterface VARIABLESSHARED_EXPORT UserFunction_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &identifier, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Function>(identifier, std::move(arguments));
        };
    fi.matches = [](const std::string &, unsigned int) {
            return true;
        };
    return fi;
}

BinaryOperatorInterface VARIABLESSHARED_EXPORT Assignment_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
            bool assignableFunction = false;
            if (typeid(*first) == typeid(CAS::Function)) {
                assignableFunction = true;
                for (const auto &arg : static_cast<CAS::Function*>(first.get())->getOperands())
                    if (typeid(*arg) != typeid(CAS::Variable)) assignableFunction = false;
            }
            if (assignableFunction || typeid(*first) == typeid(CAS::Variable) || typeid(*first) == typeid(VariableDeclarationExpression))
                return make_unique<CAS::Assignment>(std::move(first), std::move(second));
            else return std::unique_ptr<CAS::Assignment>(nullptr);
        };
    return oi;
}

FunctionInterface VARIABLESSHARED_EXPORT LazyEval_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::LazyEval>(std::move(arguments.front()));
        };
    return fi;
}

std::unique_ptr<VariableDeclarationExpression> doParseVariableDeclaration(const std::string &candidate) {
    auto spaceIt = candidate.find_first_of(' ');
    auto type = candidate.substr(0, spaceIt);
    auto ids = ExpressionParser::tokenize(candidate.substr(spaceIt + 1, std::string::npos), ",");
    for (auto &id : ids) {
        id = ExpressionParser::trim(id);
        if (! (isalpha(id.front()) && std::find_if(id.begin(), id.end(), std::not1(std::pointer_to_unary_function<int, int>(isalnum))) == id.end()))
            return nullptr;
    }
    try {
        return make_unique<VariableDeclarationExpression>(CAS::TypeInfo::fromString(std::move(type)), std::move(ids));
    } catch (const char *) {
        return nullptr;
    }
}

inline std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT VariableDeclaration_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    return doParseVariableDeclaration(candidate);
}

std::unique_ptr<CAS::AbstractExpression> doParseFunctionDeclaration(const std::string &candidate)
{
    if (candidate.back() != ')') return nullptr;
    auto spaceIt = candidate.find_first_of(' '), parenthesisIt = candidate.find_first_of('(');
    std::string type = candidate.substr(0, spaceIt), id = candidate.substr(spaceIt + 1, parenthesisIt - spaceIt - 1);
    if (! (isalpha(id.front()) && std::find_if(id.begin(), id.end(), std::not1(std::pointer_to_unary_function<int, int>(isalnum))) == id.end()))
        return nullptr;
    std::vector<CAS::TypeInfo> argTypes;
    std::vector<std::string> argNames;
    auto argTokens = ExpressionParser::tokenize(candidate.substr(parenthesisIt + 1, candidate.length() - parenthesisIt - 2), ",");
    for (const auto &token : argTokens) {
        auto tokenRes = doParseVariableDeclaration(ExpressionParser::trim(token));
        if (tokenRes == nullptr) return nullptr;
        else {
            argTypes.emplace_back(tokenRes->getType());
            argNames.emplace_back(tokenRes->getIDs().front());
        }
    }
    try {
        return make_unique<FunctionDeclarationExpression>(CAS::FunctionSignature{std::move(id), std::move(argTypes)}, CAS::TypeInfo::fromString(type), std::move(argNames));
    } catch (const char *) {
        return nullptr;
    }
}

inline std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT FunctionDeclaration_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    return doParseFunctionDeclaration(candidate);
}

std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT FunctionDefinition_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.back() != '}') return nullptr;
    int level = 0;
    auto it = candidate.cbegin();
    for (; it != candidate.cend() - 1; ++it) {
        if (*it == '(' || *it == '[')  level--;
        else if (*it == ')' || *it == ']' || *it == '}') level++;
        else if (*it == '{' && level--==0) break;
    }
    auto head = doParseFunctionDeclaration(ExpressionParser::trim({candidate.cbegin(), it}));
    if (head == nullptr) return nullptr;
    return make_unique<FunctionDefinitionExpression>(*static_cast<FunctionDeclarationExpression*>(head.get()), parseFunc({it + 1, candidate.cend() - 1}));
}

}
