#include "BinaryOperatorInterface.h"
#include "basicarith_global.h"
#include "Arithmetic/Addition.h"
#include "Arithmetic/Subtraction.h"
#include "Arithmetic/BinaryMultiplication.h"
#include "Arithmetic/Division.h"
#include "Arithmetic/NumberArith.h"
#include "Arithmetic/Exponentiation.h"
#include "Arithmetic/List.h"
#include "Integer.h"
#include "Arithmetic/Min.h"
#include "Arithmetic/Max.h"
#include "ExpressionParser.h"
#include "Arithmetic/Modulo.h"
#include "Arithmetic/Selection.h"
#include "Arithmetic/Range.h"
#include "BinaryOperatorModule.h" //wut

#include <string>
#include <iostream>
#include <QString>

extern "C" {

BinaryOperatorInterface BASICARITHSHARED_EXPORT Addition_jmodule()
{
    BinaryOperatorInterface oi;
    oi.matches = [](const std::string &input, size_t candidatePos, const ExpressionParser &parser) {
        if (input.at(candidatePos) == '+' && candidatePos != 0) {
            for (const auto &opModule : parser.getParserModules().binaryOperators)
                if (opModule.matches(input, candidatePos - 1, parser).first) return std::make_pair(false, 0);
            return std::make_pair(true, 1);
        } else return std::make_pair(false, 0);
    };

    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
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
                if (opModule.matches(input, candidatePos - 1, parser).first) return std::make_pair(false, 0);
            return std::make_pair(true, 1);
        } else return std::make_pair(false, 0);
    };

    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::Subtraction>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Multiplication_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::BinaryMultiplication>(std::move(first), std::move(second));
    };
    return oi;
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Division_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::Division>(std::move(first), std::move(second));
    };
    return oi;
}


BinaryOperatorInterface BASICARITHSHARED_EXPORT Exponentiation_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::Exponentiation>(std::move(first), std::move(second));
    };
    return oi;
}

std::unique_ptr<CAS::AbstractExpression> BASICARITHSHARED_EXPORT Number_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    if (candidate.front() == '-' || candidate.front() == '+') {
        if (candidate.size() == 1) return nullptr;
        else if ((candidate.size() != 2 && candidate.at(1) == '0') || candidate.find_first_not_of("0123456789", 1) != std::string::npos) return nullptr;
    } else if ((candidate.size() != 1 && candidate.front() == '0') || candidate.find_first_not_of("0123456789") != std::string::npos) return nullptr;

    return make_unique<CAS::NumberArith>(CAS::Integer(candidate));

}

std::unique_ptr<CAS::AbstractExpression> BASICARITHSHARED_EXPORT Pi_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    if (candidate == "pi") return make_unique<CAS::NumberArith>(3);
    else return nullptr;
}

std::unique_ptr<CAS::AbstractExpression> BASICARITHSHARED_EXPORT List_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.front() != '[' || candidate.back() != ']') return nullptr;

    int level = 0;
    for (auto it = candidate.begin() + 1; it != candidate.end() - 1; ++it) {
        if (*it == '[') level++;
        else if (*it == ']' && --level == -1) return nullptr;
    }

    std::vector<std::unique_ptr<CAS::AbstractExpression>> result;
    if (candidate.at(1) == '[') {
        auto lastPos = candidate.cbegin();
        level = 0;
        for (auto it = candidate.cbegin() + 1; it != candidate.cend() - 1; ++it) {
            if (*it == '(' || *it == '[' || *it == '{')  level--;
            else if (*it == ')' || *it == ']' || *it == '}') level++;
            if (level == 0) {
                result.emplace_back(parseFunc(std::string{lastPos + 1, it + 1}));
                lastPos = it;
            }
        }
    } else {
        std::vector<std::string> tokens = ExpressionParser::tokenize(std::string{candidate.cbegin() + 1, candidate.cend() - 1}, ",");
        for (const auto &token : tokens) result.emplace_back(parseFunc(token));
    }
    if (result.empty()) return nullptr;
    else return make_unique<CAS::List>(std::move(result));
}

BinaryOperatorInterface BASICARITHSHARED_EXPORT Modulo_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
        return make_unique<CAS::Modulo>(std::move(first), std::move(second));
    };
    return oi;
}

FunctionInterface BASICARITHSHARED_EXPORT Min_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Min>(std::move(arguments.front()), std::move(arguments.at(1)));
        };
    return fi;
}

FunctionInterface BASICARITHSHARED_EXPORT Max_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Max>(std::move(arguments.front()), std::move(arguments.at(1)));
        };
    return fi;
}

std::unique_ptr<CAS::AbstractExpression> BASICARITHSHARED_EXPORT Selection_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
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
    std::unique_ptr<CAS::AbstractExpression> tmpToken;
    for (const auto &token : ExpressionParser::tokenize(candidate.substr(i + 1, candidate.length() - i - 2), ","))
        if (tmpToken = parseFunc(token)) selectTokens.emplace_back(std::move(tmpToken));
    return make_unique<CAS::Selection>(parseFunc(candidate.substr(0, i)), std::move(selectTokens));
}

FunctionInterface BASICARITHSHARED_EXPORT Range_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Range>(std::move(arguments.front()), std::move(arguments.at(1)), make_unique<CAS::NumberArith>(1));
        };
    return fi;
}

FunctionInterface BASICARITHSHARED_EXPORT RangeCStep_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Range>(std::move(arguments.front()), std::move(arguments.at(1)), std::move(arguments.at(2)));
        };
    return fi;
}

}

