#include "flowcontrol_global.h"
#include "ExpressionParser.h"
#include "lang/ScopeStatement.h"
#include "lang/BoolValue.h"
#include "lang/NumberValue.h"
#include "lang/If.h"
#include "lang/Equality.h"
#include "lang/Negation.h"
#include "lang/LogicalAnd.h"
#include "lang/LogicalOr.h"
#include "lang/Less.h"
#include "lang/LessOrEqual.h"
#include "lang/Greater.h"
#include "lang/GreaterOrEqual.h"
#include "lang/RangedFor.h"
#include "lang/MultiLine.h"
#include "lang/Import.h"
#include "lang/Function.h"

extern "C" {

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT Scope_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (candidate.front() != '{' || candidate.back() != '}') return nullptr;
    else return make_unique<CAS::ScopeStatement>(parseStatementFunc({candidate.cbegin() + 1, candidate.cend() - 1}));
}

CAS::AbstractExpression::ExpressionP FLOWCONTROLSHARED_EXPORT Bool_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (candidate == "true") return make_unique<CAS::BoolValue>(true);
    else if (candidate == "false") return make_unique<CAS::BoolValue>(false);
    else return nullptr;
}

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT If_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    if (candidate.substr(0,4) != "if (") return nullptr;
    else {
        auto itBody = candidate.cbegin() + 4;
        std::vector<std::pair<CAS::AbstractExpression::ExpressionP, CAS::AbstractStatement::StatementP>> conditionals;
        for (;;) {
            auto level = 0;
            auto itCondition = itBody;
            for (; itCondition != candidate.cend(); ++itCondition) {
                if (*itCondition == '(')  level--;
                else if (*itCondition == ')' && ++level == 1) break;
            }
            auto condition = parseExpressionFunc({itBody, itCondition}); //try catch
            level = 0;
            itBody = itCondition + 1;
            for (; itBody != candidate.cend(); ++itBody) {
                if (*itBody == '(' || *itBody == '[' || *itBody == '{')  level--;
                else if (*itBody == ')' || *itBody == ']' || *itBody == '}') level++;
                else if (level == 0) {
                    if (std::equal(itBody, itBody + 9, std::string("else if (").cbegin())) {
                        conditionals.emplace_back(std::move(condition), parseStatementFunc({itCondition + 1, itBody}));
                        itBody += 9;
                        break;
                    } else if (std::equal(itBody, itBody + 4, std::string("else").cbegin())) {
                        conditionals.emplace_back(std::move(condition), parseStatementFunc({itCondition + 1, itBody}));
                        conditionals.emplace_back(make_unique<CAS::BoolValue>(true), parseStatementFunc({itBody + 4, candidate.cend()}));
                        return make_unique<CAS::If>(std::move(conditionals));
                    }
                }
            }
            if (itBody == candidate.cend()) {
                conditionals.emplace_back(std::move(condition), parseStatementFunc({itCondition + 1, candidate.cend()}));
                return make_unique<CAS::If>(std::move(conditionals));
            }
        }
    }
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Equality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Equality>(std::move(first), std::move(second));
    };
    return oi;
}

UnaryOperatorInterface FLOWCONTROLSHARED_EXPORT Negation_jmodule()
{
    UnaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP operand) {
        return make_unique<CAS::Negation>(std::move(operand));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Inequality_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Negation>(make_unique<CAS::Equality>(std::move(first), std::move(second)));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalAnd_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::LogicalAnd>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LogicalOr_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::LogicalOr>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Less_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Less>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT LessOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::LessOrEqual>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Greater_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Greater>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT GreaterOrEqual_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::GreaterOrEqual>(std::move(first), std::move(second));
    };
    return oi;
}

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT RangedFor_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
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
            return make_unique<CAS::RangedFor>(parseStatementFunc({candidate.cbegin() + 5, sepPos}), parseExpressionFunc({sepPos + 1, it}), parseStatementFunc({it + 1, candidate.cend()}));
        }
    }
    return nullptr;
}

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT MultiLine_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    CAS::AbstractStatement::Statements tokens;
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
                if (! token.empty()) tokens.emplace_back(make_unique<CAS::Print>(parseExpressionFunc(token)));
                lastPos = it + 1;
            } else if (*it == ';') {
                token = {lastPos, it};
                token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
                token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
                if (! token.empty()) tokens.emplace_back(parseStatementFunc(token));
                lastPos = it + 1;
            }
        }
    }
    if (lastPos != candidate.cbegin()) {
        token = {lastPos, candidate.cend()};
        token.erase(begin(token), std::find_if_not(begin(token), end(token), isspace));
        token.erase(std::find_if_not(token.rbegin(), token.rend(), isspace).base(), end(token));
        if (! token.empty()) tokens.emplace_back(make_unique<CAS::Print>(parseExpressionFunc(token)));
        return make_unique<CAS::MultiLine>(std::move(tokens));
    } else return nullptr;
}

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT Return_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    if (candidate.substr(0, 7) != "return ") return nullptr;
    return make_unique<CAS::Return>(parseExpressionFunc({candidate.cbegin() + 7, candidate.cend()}));
}

CAS::AbstractStatement::StatementP FLOWCONTROLSHARED_EXPORT Import_jmodule(const std::string &candidate, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (candidate.substr(0, 7) != "import ") return nullptr;
    return make_unique<CAS::Import>(std::string{candidate.cbegin() + 7, candidate.cend()});
}

BinaryOperatorInterface FLOWCONTROLSHARED_EXPORT Dot_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) -> CAS::AbstractExpression::ExpressionP {
        if (typeid(*second) != typeid(CAS::Function)) return nullptr;
        static_cast<CAS::Function*>(second.get())->getOperands().emplace(static_cast<CAS::Function*>(second.get())->getOperands().begin(), std::move(first));
        return second;
    };
    return oi;
}

}
