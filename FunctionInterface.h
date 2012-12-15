#ifndef FUNCTIONINTERFACE_H
#define FUNCTIONINTERFACE_H

#include <functional>
#include "lang/AbstractExpression.h"

/**
 * Interface for function parser modules containing function pointers
 */
struct FunctionInterface
{
    std::function<bool(const std::string &, unsigned int)> matches;
    std::function<CAS::AbstractExpression::ExpressionP(std::string &, CAS::AbstractExpression::Expressions &)> parse;
    std::function<unsigned int()> priority;
};

#endif // FUNCTIONINTERFACE_H
