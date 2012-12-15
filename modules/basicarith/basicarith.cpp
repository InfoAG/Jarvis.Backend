#include "BinaryOperatorInterface.h"
#include "basicarith_global.h"
#include "lang/Addition.h"
#include "lang/Subtraction.h"
#include "lang/BinaryMultiplication.h"
#include "lang/Division.h"
#include "lang/NumberValue.h"
#include "lang/Exponentiation.h"
#include "lang/List.h"
#include "ExpressionParser.h"
#include "lang/Modulo.h"
#include "BinaryOperatorModule.h"
#include "lang/Factorial.h"
#include "lang/Vector.h"
#include "FunctionInterface.h"
#include "lang/NaturalLogarithm.h"

#include <string>
#include <iostream>
#include <QString>
#include <sstream>

extern "C" {

BinaryOperatorInterface BASICARITHSHARED_EXPORT Addition_jmodule()
{
    BinaryOperatorInterface oi;
    oi.matches = [](const std::string &input, size_t candidatePos, const ExpressionParser &parser) {
        if (input.at(candidatePos) == '+' && candidatePos != 0) {
            for (const auto &opModule : parser.getParserModules().binaryOperators)
                if (opModule->matches(input, candidatePos - 1, parser).first) return std::make_pair(false, 0);
            return std::make_pair(true, 1);
        } else return std::make_pair(false, 0);
    };

    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Addition>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Subtraction_jmodule()
{
    BinaryOperatorInterface oi;
    oi.matches = [](const std::string &input, size_t candidatePos, const ExpressionParser &parser) {
        if (input.at(candidatePos) == '-' && candidatePos != 0) {
            for (const auto &opModule : parser.getParserModules().binaryOperators)
                if (opModule->matches(input, candidatePos - 1, parser).first) return std::make_pair(false, 0);
            return std::make_pair(true, 1);
        } else return std::make_pair(false, 0);
    };

    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Subtraction>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Multiplication_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::BinaryMultiplication>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Division_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Division>(std::move(first), std::move(second));
    };
    return oi;
}


BinaryOperatorInterface BASICARITHSHARED_EXPORT Exponentiation_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Exponentiation>(std::move(first), std::move(second));
    };
    return oi;
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Number_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    /*if (candidate.front() == '-' || candidate.front() == '+') {
        if (candidate.size() == 1) return nullptr;
        else if ((candidate.size() != 2 && candidate.at(1) == '0') || candidate.find_first_not_of("0123456789", 1) != std::string::npos) return nullptr;
    } else if ((candidate.size() != 1 && candidate.front() == '0') || candidate.find_first_not_of("0123456789") != std::string::npos) return nullptr;
    */

    std::istringstream ss(candidate);
    double value;
    if (! (ss >> value)) return nullptr;
    std::ostringstream os;
    os << value;
    if (os.str() != candidate) return nullptr;
    return make_unique<CAS::NumberValue>(value);
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Pi_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (candidate == "pi") return make_unique<CAS::Constant>(CAS::Constant::PI);
    else return nullptr;
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Euler_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)
{
    if (candidate == "e") return make_unique<CAS::Constant>(CAS::Constant::EULER);
    else return nullptr;
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT List_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    if (candidate.front() != '[' || candidate.back() != ']') return nullptr;

    int level = 0;
    for (auto it = candidate.begin() + 1; it != candidate.end() - 1; ++it) {
        if (*it == '[') level++;
        else if (*it == ']' && --level == -1) return nullptr;
    }

    std::vector<CAS::AbstractExpression::ExpressionP> result;
    if (candidate.at(1) == '[') {
        auto lastPos = candidate.cbegin();
        level = 0;
        for (auto it = candidate.cbegin() + 1; it != candidate.cend() - 1; ++it) {
            if (*it == '(' || *it == '[' || *it == '{')  level--;
            else if (*it == ')' || *it == ']' || *it == '}') level++;
            if (level == 0) {
                result.emplace_back(parseExpressionFunc(std::string{lastPos + 1, it + 1}));
                lastPos = it;
            }
        }
    } else {
        std::vector<std::string> tokens = ExpressionParser::tokenize(std::string{candidate.cbegin() + 1, candidate.cend() - 1}, ",");
        for (const auto &token : tokens) result.emplace_back(parseExpressionFunc(token));
    }
    if (result.empty()) return nullptr;
    else return make_unique<CAS::List>(std::move(result));
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Modulo_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP first, CAS::AbstractExpression::ExpressionP second) {
        return make_unique<CAS::Modulo>(std::move(first), std::move(second));
    };
    return oi;
}
/*
CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Selection_jmodule(const std::string &candidate, std::function<CAS::AbstractExpression::ExpressionP(std::string)> parseFunc)
{
    if (candidate.back() != '}') return nullptr;
    int level = 0, i = candidate.size() - 2;
    for (; i != -1; i--) {
        if (level == 0 && candidate.at(i) == '{') break;
        else if (candidate.at(i) == '(' || candidate.at(i) == '[' || candidate.at(i) == '{')  level--;
        else if (candidate.at(i) == ')' || candidate.at(i) == ']' || candidate.at(i) == '}') level++;
    }
    if (i == -1) return nullptr;
    CAS::AbstractExpression::Operands selectTokens;
    CAS::AbstractExpression::ExpressionP tmpToken;
    for (const auto &token : ExpressionParser::tokenize(candidate.substr(i + 1, candidate.length() - i - 2), ","))
        if (tmpToken = parseFunc(token)) selectTokens.emplace_back(std::move(tmpToken));
    return make_unique<CAS::Selection>(parseFunc(candidate.substr(0, i)), std::move(selectTokens));
}
*/
UnaryOperatorInterface BASICARITHSHARED_EXPORT Factorial_jmodule()
{
    UnaryOperatorInterface oi;
    oi.parse = [](CAS::AbstractExpression::ExpressionP operand) {
        return make_unique<CAS::Factorial>(std::move(operand));
    };
    return oi;
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Vector_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    if (candidate.front() != '(' || candidate.back() != ')') return nullptr;

    auto tokens = ExpressionParser::tokenize({candidate.cbegin() + 1, candidate.cend() - 1}, ",");
    if (tokens.size() != 3) return nullptr;
    return make_unique<CAS::Vector>(parseExpressionFunc(tokens.front()), parseExpressionFunc(tokens[1]), parseExpressionFunc(tokens.back()));
}

CAS::AbstractExpression::ExpressionP BASICARITHSHARED_EXPORT Parenthesis_jmodule(const std::string &candidate, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc)
{
    if (candidate.front() != '(' || candidate.back() != ')') return nullptr;
    return parseExpressionFunc({candidate.cbegin() + 1, candidate.cend() - 1});
}

FunctionInterface BASICARITHSHARED_EXPORT NaturalLogarithm_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](std::string &, CAS::AbstractExpression::Expressions &arguments) {
            return make_unique<CAS::NaturalLogarithm>(std::move(arguments.front()));
        };
    return fi;
}

}

