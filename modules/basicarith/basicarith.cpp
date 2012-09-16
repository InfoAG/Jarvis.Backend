#include "OperatorInterface.h"
#include "basicarith_global.h"
#include "Arithmetic/Addition.h"
#include "Arithmetic/Subtraction.h"
#include "Arithmetic/Multiplication.h"
#include "Arithmetic/Division.h"
#include "Arithmetic/NumberArith.h"
#include "Arithmetic/Exponentiation.h"
#include "Arithmetic/Matrix.h"
#include "Natural.h"
#include "Arithmetic/Min.h"
#include "Arithmetic/Max.h"
#include "ExpressionParser.h"
#include "Arithmetic/Modulo.h"

#include <string>
#include <iostream>
#include <QString>

extern "C" {

OperatorInterface BASICARITHSHARED_EXPORT Addition_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Addition>(std::move(first), std::move(second));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Subtraction_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Subtraction>(std::move(first), std::move(second));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Multiplication_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Multiplication>(std::move(first), std::move(second));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Division_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Division>(std::move(first), std::move(second));
    };
    return oi;
}


OperatorInterface BASICARITHSHARED_EXPORT Exponentiation_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Exponentiation>(std::move(first), std::move(second));
    };
    return oi;
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Number_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)>)
{
    if (candidate.size() != 1 && candidate.front() == '0') return nullptr;
    else {
        if (candidate.find_first_not_of("0123456789") != std::string::npos) return nullptr;
        else return make_unique<CAS::NumberArith>(CAS::Natural(candidate));
    }
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Pi_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)>)
{
    if (candidate == "pi") return make_unique<CAS::NumberArith>(3);
    else return nullptr;
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Matrix_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)> parseFunc)
{
    if (candidate.front() != '[' || candidate.back() != ']') return nullptr;

    int level = 0;
    for (auto it = candidate.begin() + 1; it != candidate.end() - 1; ++it) {
        if (*it == '[') level++;
        else if (*it == ']' && --level == -1) return nullptr;
    }

    std::vector<std::unique_ptr<CAS::AbstractArithmetic>> result;
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
    else return make_unique<CAS::Matrix>(std::move(result));
}

OperatorInterface BASICARITHSHARED_EXPORT Modulo_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return make_unique<CAS::Modulo>(std::move(first), std::move(second));
    };
    return oi;
}

FunctionInterface BASICARITHSHARED_EXPORT Min_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractArithmetic>> &arguments) {
            return make_unique<CAS::Min>(std::move(arguments.front()), std::move(arguments.at(1)));
        };
    return fi;
}

FunctionInterface BASICARITHSHARED_EXPORT Max_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractArithmetic>> &arguments) {
            return make_unique<CAS::Max>(std::move(arguments.front()), std::move(arguments.at(1)));
        };
    return fi;
}

}

