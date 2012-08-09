#ifndef OPERATORINTERFACE_H
#define OPERATORINTERFACE_H

#include <functional>
#include <string>
#include <memory>
#include "Arithmetic/AbstractArithmetic.h"

struct OperatorInterface
{
    enum AssociativityType { LEFT, RIGHT };
/*
    bool (*matches)(std::string);
    int (*priority)();
    AssociativityType (*associativity)();
    std::unique_ptr<CAS::AbstractArithmetic> (*parse)(std::unique_ptr<CAS::AbstractArithmetic>, std::unique_ptr<CAS::AbstractArithmetic>);
    */

    std::function<bool(std::string)> matches;
    std::function<int()> priority;
    std::function<AssociativityType()> associativity;
    std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::unique_ptr<CAS::AbstractArithmetic>, std::unique_ptr<CAS::AbstractArithmetic>)> parse;
};

#endif // OPERATORINTERFACE_H
