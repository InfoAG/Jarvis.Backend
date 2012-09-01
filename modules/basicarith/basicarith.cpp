#include "OperatorInterface.h"
#include "basicarith_global.h"
#include "Arithmetic/Addition.h"
#include "Arithmetic/Subtraction.h"
#include "Arithmetic/Multiplication.h"
#include "Arithmetic/Division.h"
#include "Arithmetic/NumberArith.h"
#include "Arithmetic/Exponentiation.h"

#include <string>
#include <iostream>
#include <QString>
//#pragma weak operator_factory
//extern std::map<std::string, int> operator_factory;
/*
std::unique_ptr<CAS::AbstractArithmetic> parse()
{

}*/

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

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Number_jmodule(const std::string &candidate)
{
    bool ok;
    int num = QString::fromStdString(candidate).toInt(&ok);
    if (ok) return make_unique<CAS::NumberArith>(num);
    else return nullptr;
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Pi_jmodule(const std::string &candidate)
{
    if (candidate == "pi") return make_unique<CAS::NumberArith>(3);
    else return nullptr;
}


}

