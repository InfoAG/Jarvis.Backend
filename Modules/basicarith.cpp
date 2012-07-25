#include "../OperatorInterface.h"
#include "basicarith_global.h"
#include "../../Framework/Arithmetic/Addition.h"
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
/*
    oi.matches = [](std::string candidate) {
        static int count = 0;
        std::cout << count << "\t" << candidate << std::endl;
        if (count == 0) {
            if (candidate == "*") {
                count++;
                return true;
            } else return false;
        } else return candidate == "+";
    };*/
    return oi;
}

std::unique_ptr<CAS::AbstractArithmetic> BASICARITHSHARED_EXPORT Number_maker(std::string candidate)
{
    bool ok;
    unsigned int num = QString::fromStdString(candidate).toUInt(&ok);
    if (ok) return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::NumberArith(num));
    else return nullptr;
}

}

