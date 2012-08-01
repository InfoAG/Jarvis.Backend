#include "../OperatorInterface.h"
#include "basicarith_global.h"
#include "../../Framework/Arithmetic/Addition.h"
#include "../../Framework/Arithmetic/Subtraction.h"
#include "../../Framework/Arithmetic/Multiplication.h"
#include "../../Framework/Arithmetic/Division.h"
#include "../../Framework/Arithmetic/NumberArith.h"

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

OperatorInterface BASICARITHSHARED_EXPORT Addition_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Addition(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Subtraction_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Subtraction(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Multiplication_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Multiplication(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
    };
    return oi;
}

OperatorInterface BASICARITHSHARED_EXPORT Division_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Division(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
    };
    return oi;
}
/*
OperatorInterface BASICARITHSHARED_EXPORT Exponentiation_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Exponentiation(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
    };
    return oi;
}
*/
std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Number_maker(std::string candidate)
{
    bool ok;
    int num = QString::fromStdString(candidate).toInt(&ok);
    if (ok) return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::NumberArith(num));
    else return nullptr;
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Pi_maker(std::string candidate)
{
    if (candidate == "pi") return std::unique_ptr<CAS::NumberArith>(new CAS::NumberArith(3));
    else return nullptr;
}


}

