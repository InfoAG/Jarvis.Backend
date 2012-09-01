#ifndef FUNCTIONINTERFACE_H
#define FUNCTIONINTERFACE_H

#include <functional>

/**
 * Interface for function parser modules containing function pointers
 */
struct FunctionInterface
{
    std::function<bool(const std::string &, unsigned int)> matches;
    std::function<std::unique_ptr<CAS::AbstractArithmetic>(const std::string &, std::vector<std::unique_ptr<CAS::AbstractArithmetic>> &)> parse;
    std::function<unsigned int()> priority;
};

#endif // FUNCTIONINTERFACE_H
