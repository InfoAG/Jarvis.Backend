#include "BinaryOperatorInterface.h"
#include "FunctionInterface.h"
#include "lang/AbstractExpression.h"
#include "lang/Variable.h"
#include "lang/Assignment.h"
#include "lang/Function.h"
#include "lang/LazyEval.h"
#include <memory>
#include "variables_global.h"
#include "lang/VariableDeclaration.h"
#include "ExpressionParser.h"
#include "lang/FunctionDeclaration.h"
#include "lang/FunctionDefinitionStatement.h"

extern "C" {

CAS::AbstractExpression::ExpressionP VARIABLESSHARED_EXPORT Variable_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (isalpha(candidate.front()) && std::find_if(candidate.begin() + 1, candidate.end(), std::not1(std::pointer_to_unary_function<int, int>(isalnum))) == candidate.end())
        return make_unique<CAS::Variable>(candidate);
    else return nullptr;
}

FunctionInterface VARIABLESSHARED_EXPORT UserFunction_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &identifier, CAS::AbstractExpression::Expressions &arguments) {
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
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
            bool assignableFunction = false;
            if (typeid(*first) == typeid(CAS::Function)) {
                assignableFunction = true;
                for (const auto &arg : static_cast<CAS::Function*>(first.get())->getOperands())
                    if (typeid(*arg) != typeid(CAS::Variable)) assignableFunction = false;
            }
            if (assignableFunction || typeid(*first) == typeid(CAS::Variable) || typeid(*first) == typeid(CAS::VariableDeclaration))
                return make_unique<CAS::Assignment>(std::move(first), std::move(second));
            else return std::unique_ptr<CAS::Assignment>(nullptr);
        };
    return oi;
}

FunctionInterface VARIABLESSHARED_EXPORT LazyEval_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<CAS::AbstractExpression::ExpressionP> &arguments) {
            return make_unique<CAS::LazyEval>(std::move(arguments.front()));
        };
    return fi;
}

std::unique_ptr<CAS::VariableDeclaration> doParseVariableDeclaration(const std::string &candidate) {
    auto spaceIt = candidate.find_first_of(' ');
    auto type = candidate.substr(0, spaceIt);
    auto ids = ExpressionParser::tokenize(candidate.substr(spaceIt + 1, std::string::npos), ",");
    for (auto &id : ids) {
        id = ExpressionParser::trim(id);
        if (! (isalpha(id.front()) && std::find_if(id.begin() + 1, id.end(), std::not1(std::pointer_to_unary_function<int, int>(isalnum))) == id.end()))
            return nullptr;
    }
    try {
        return make_unique<CAS::VariableDeclaration>(CAS::TypeInfo::fromString(std::move(type)), std::move(ids));
    } catch (const CAS::FatalTypeException &) {
        return nullptr;
    }
}

CAS::AbstractStatement::StatementP VARIABLESSHARED_EXPORT VariableDeclaration_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    return doParseVariableDeclaration(candidate);
}

std::unique_ptr<CAS::FunctionDeclaration> doParseFunctionDeclaration(const std::string &candidate)
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
        return make_unique<CAS::FunctionDeclaration>(CAS::FunctionSignature{std::move(id), std::move(argTypes)}, CAS::TypeInfo::fromString(type), std::move(argNames));
    } catch (CAS::FatalTypeException &) {
        return nullptr;
    }
}

CAS::AbstractStatement::StatementP VARIABLESSHARED_EXPORT FunctionDeclaration_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    return doParseFunctionDeclaration(candidate);
}

CAS::AbstractStatement::StatementP VARIABLESSHARED_EXPORT FunctionDefinition_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
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
    return make_unique<CAS::FunctionDefinitionStatement>(*head, parseStatementFunc({it + 1, candidate.cend() - 1}));
}

}
