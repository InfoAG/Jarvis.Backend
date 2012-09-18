#ifndef OPERATORINTERFACE_H
#define OPERATORINTERFACE_H

#include <functional>
#include <string>
#include <memory>
#include "Arithmetic/AbstractArithmetic.h"

class ExpressionParser;

struct OperatorInterface
{
    enum AssociativityType { LEFT, RIGHT };

    std::function<bool(const std::string&, size_t, const ExpressionParser&)> matches;
    std::function<unsigned int()> priority;
    std::function<AssociativityType()> associativity;
    std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::unique_ptr<CAS::AbstractArithmetic>, std::unique_ptr<CAS::AbstractArithmetic>)> parse;
};

#endif // OPERATORINTERFACE_H
