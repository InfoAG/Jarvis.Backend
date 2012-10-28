#include "flowcontrol_global.h"
#include "ExpressionParser.h"
#include "ScopeExpression.h"
#include "BoolValue.h"
#include "Arithmetic/NumberArith.h"
#include "IfExpression.h"
#include "EqualityExpression.h"
#include "NegationExpression.h"
#include "LogicalAndExpression.h"
#include "LogicalOrExpression.h"
#include "LessExpression.h"
#include "LessOrEqualExpression.h"
#include "GreaterExpression.h"
#include "GreaterOrEqualExpression.h"
#include "RangedForExpression.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Scope_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.front() != '{' || candidate.back() != '}') return nullptr;
    int level = 0;
    for (auto it = candidate.begin() + 1; it != candidate.end() - 1; ++it) {
        if (*it == '{') level++;
        else if (*it == '}' && --level == -1) return nullptr;
    }

    CAS::AbstractExpression::Operands tokens;
    level = 0;
    std::string token;
    auto lastPos = candidate.cbegin() + 1;
    for (auto it = candidate.cbegin() + 1; it != candidate.cend() - 1; ++it) {
        if (*it == '(' || *it == '[' || *it == '{')  level--;
        else if (*it == ')' || *it == ']' || *it == '}') level++;
        else if (level == 0) {
            if (*it == '\n') {
                token = {lastPos, it};
                token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
                token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
                if (! token.empty()) tokens.emplace_back(make_unique<OutputExpression>(parseFunc(token)));
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
    if (lastPos != candidate.cbegin() + 1) {
        token = {lastPos, candidate.cend() - 1};
        token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
        token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
        if (! token.empty()) tokens.emplace_back(make_unique<OutputExpression>(parseFunc(token)));
    } else tokens.emplace_back(make_unique<OutputExpression>(parseFunc({candidate.cbegin() + 1, candidate.cend() - 1})));
    return make_unique<ScopeExpression>(std::move(tokens));
}

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Bool_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    if (candidate == "true") return make_unique<BoolValue>(true);
    else if (candidate == "false") return make_unique<BoolValue>(false);
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
                        conditionals.emplace_back(make_unique<BoolValue>(true), parseFunc({itBody + 4, candidate.cend()}));
                        return make_unique<IfExpression>(std::move(conditionals));
                    }
                }
            }
            if (itBody == candidate.cend()) {
                conditionals.emplace_back(std::move(condition), parseFunc({itCondition + 1, candidate.cend()}));
                return make_unique<IfExpression>(std::move(conditionals));
            }
        }
    }
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Equality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<EqualityExpression>(std::move(first), std::move(second));
    };
    return oi;
}

UnaryOperatorInterface FLOWCONTROLSHARED_EXPORT Negation_jmodule()
{
    UnaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> operand) {
        return make_unique<NegationExpression>(std::move(operand));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Inequality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<NegationExpression>(make_unique<EqualityExpression>(std::move(first), std::move(second)));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalAnd_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<LogicalAndExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalOr_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<LogicalOrExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Less_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<LessExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LessOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<LessOrEqualExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Greater_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<GreaterExpression>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT GreaterOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<GreaterOrEqualExpression>(std::move(first), std::move(second));
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
            return make_unique<RangedForExpression>(parseFunc({candidate.cbegin() + 5, sepPos}), parseFunc({sepPos + 1, it}), parseFunc({it + 1, candidate.cend()}));
        }
    }
    return nullptr;
}

}
