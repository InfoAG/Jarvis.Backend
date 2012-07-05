#ifndef ABSTRACTOPERATOR_H
#define ABSTRACTOPERATOR_H

#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include <string>

class AbstractOperator
{
public:
    virtual bool matches(std::string) const = 0;
    virtual unsigned int priority() const = 0;
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic>, std::unique_ptr<CAS::AbstractArithmetic>) const = 0;
};

#endif //ABSTRACTFUNCTION_H
