#ifndef DEMOMODULES_H
#define DEMOMODULES_H

#include "AbstractOperator.h"
#include "AbstractTerminal.h"
#include "../Framework/Arithmetic/Addition.h"

class AdditionExpression : public AbstractOperator
{
public:
    virtual bool matches(std::string candidate) { return candidate == "+"; };
    virtual unsigned int priority() { return 0; }
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) { return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Addition(std::move(first), std::move(second))); };
};

class NumberExpression : public AbstractTerminal
{
public:
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::string candidate) { if (candidate == "3") return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::NumberArith(3)); else return 0; };
};

#endif //DEMOMODULES_H
