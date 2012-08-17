#ifndef FUNCTIONINTERFACE_H
#define FUNCTIONINTERFACE_H

#include <functional>

/**
 * Interface for function parser modules containing function pointers
 */
struct FunctionInterface
{
    std::function<bool(std::string)> matches;
    std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::unique_ptr<CAS::AbstractArithmetic>)> parse;
};

#endif // FUNCTIONINTERFACE_H
