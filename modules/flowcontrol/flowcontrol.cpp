#include "flowcontrol_global.h"
#include "ExpressionParser.h"
#include "expression/ScopeExpression.h"
#include "expression/BoolValue.h"
#include "expression/NumberValue.h"
#include "expression/IfExpression.h"
#include "expression/EqualityExpression.h"
#include "expression/NegationExpression.h"
#include "expression/LogicalAndExpression.h"
#include "expression/LogicalOrExpression.h"
#include "expression/LessExpression.h"
#include "expression/LessOrEqualExpression.h"
#include "expression/GreaterExpression.h"
#include "expression/GreaterOrEqualExpression.h"
#include "expression/RangedForExpression.h"
#include "expression/MultiLineExpression.h"
#include "expression/ImportExpression.h"
#include "expression/Function.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Scope_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.front() != '{' || candidate.back() != '}') return nullptr;
    else return make_unique<CAS::ScopeExpression>(parseFunc({candidate.cbegin() + 1, candidate.cend() - 1}));
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Bool_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    if (candidate == "true") return make_unique<CAS::BoolValue>(true);
    else if (candidate == "false") return make_unique<CAS::BoolValue>(false);
    else return nullptr;
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT If_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.substr(0,4) != "if (") return nullptr;
    else {
        auto itBody = candidate.cbegin() + 4;
        std::vector<std::pair<CAS::AbstractExpression::ExpressionP, CAS::AbstractExpression::ExpressionP>> conditionals;
        for (;;) {
            auto level = 0;
            auto itCondition = itBody;
            for (; itCondition != candidate.cend(); ++itCondition) {
                if (*itCondition == '(')  level--;
                else if (*itCondition == ')' && ++level == 1) break;
            }
            auto condition = parseFunc({itBody, itCondition}); //try catch
            level = 0;
            itBody = itCondition + 1;
            for (; itBody != candidate.cend(); ++itBody) {
                if (*itBody == '(' || *itBody == '[' || *itBody == '{')  level--;
                else if (*itBody == ')' || *itBody == ']' || *itBody == '}') level++;
                else if (level == 0) {
                    if (std::equal(itBody, itBody + 9, std::string("else if (").cbegin())) {
                        conditionals.emplace_back(std::move(condition), parseFunc({itCondition + 1, itBody}));
                        itBody += 9;
                        break;
                    } else if (std::equal(itBody, itBody + 4, std::string("else").cbegin())) {
                        conditionals.emplace_back(std::move(condition), parseFunc({itCondition + 1, itBody}));
                        conditionals.emplace_back(make_unique<CAS::BoolValue>(true), parseFunc({itBody + 4, candidate.cend()}));
                        return make_unique<CAS::IfExpression>(std::move(conditionals));
                    }
                }
            }
            if (itBody == candidate.cend()) {
                conditionals.emplace_back(std::move(condition), parseFunc({itCondition + 1, candidate.cend()}));
                return make_unique<CAS::IfExpression>(std::move(conditionals));
            }
        }
    }
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Equality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::EqualityExpression>(std::move(first), std::move(second));
    };
    return oi;
}

UnaryOperatorInterface FLOWCONTROLSHARED_EXPORT Negation_jmodule()
{
    UnaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> operand) {
        return make_unique<CAS::NegationExpression>(std::move(operand));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Inequality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::NegationExpression>(make_unique<CAS::EqualityExpression>(std::move(first), std::move(second)));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalAnd_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::LogicalAndExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalOr_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::LogicalOrExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Less_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::LessExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LessOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::LessOrEqualExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Greater_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::GreaterExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT GreaterOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::GreaterOrEqualExpression>(std::move(first), std::move(second));
    };
    return oi;
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT RangedFor_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.substr(0, 5) != "for (") return nullptr;
    int level = 0;
    auto sepPos = candidate.cend();
    for (auto it = candidate.cbegin() + 5; it != candidate.cend(); ++it) {
        if (*it == '(' || *it == '[' || *it == '{')  level--;
        else if (*it == ']' || *it == '}') level++;
        else if (level == 0 && *it == ':') sepPos = it;
        else if (*it == ')' && ++level == 1) {
            if (sepPos == candidate.cend()) return nullptr;
            return make_unique<CAS::RangedForExpression>(parseFunc({candidate.cbegin() + 5, sepPos}), parseFunc({sepPos + 1, it}), parseFunc({it + 1, candidate.cend()}));
        }
    }
    return nullptr;
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT MultiLine_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    CAS::AbstractExpression::Operands tokens;
    int level = 0;
    std::string token;
    auto lastPos = candidate.cbegin();
    for (auto it = candidate.cbegin(); it != candidate.cend(); ++it) {
        if (*it == '(' || *it == '[' || *it == '{')  level--;
        else if (*it == ')' || *it == ']' || *it == '}') level++;
        else if (level == 0) {
            if (*it == '\n') {
                token = {lastPos, it};
                token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
                token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
                if (! token.empty()) tokens.emplace_back(make_unique<CAS::OutputExpression>(parseFunc(token)));
                lastPos = it + 1;
            } else if (*it == ';') {
                token = {lastPos, it};
                token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
                token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
                if (! token.empty()) tokens.emplace_back(parseFunc(token));
                lastPos = it + 1;
            }
        }
    }
    if (lastPos != candidate.cbegin()) {
        token = {lastPos, candidate.cend()};
        token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
        token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
        if (! token.empty()) tokens.emplace_back(make_unique<CAS::OutputExpression>(parseFunc(token)));
        return make_unique<CAS::MultiLineExpression>(std::move(tokens));
    } else return nullptr;
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Return_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.substr(0, 7) != "return ") return nullptr;
    return make_unique<CAS::ReturnExpression>(parseFunc({candidate.cbegin() + 7, candidate.cend()}));
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Import_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    if (candidate.substr(0, 7) != "import ") return nullptr;
    return make_unique<CAS::ImportExpression>(std::string{candidate.cbegin() + 7, candidate.cend()});
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Dot_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) -> CAS::AbstractExpression::ExpressionP {
        if (typeid(*second) != typeid(CAS::Function)) return nullptr;
        static_cast<CAS::Function*>(second.get())->getOperands().emplace(static_cast<CAS::Function*>(second.get())->getOperands().begin(), std::move(first));
        return second;
    };
    return oi;
}

}
