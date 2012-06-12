#ifndef DEMOMODULES_H
#define DEMOMODULES_H

#include "AbstractOperator.h"
#include "AbstractTerminal.h"
#include "Framework/Arithmetic/Addition.h"

namespace CAS {

class Number : public AbstractArithmetic
{
private:
    int n;

public:
    Number(int n) : n(n) {};
    virtual inline AbstractArithmetic *copy() const { return new Number(*this); };

    virtual inline AbstractArithmetic *eval(const EvalInfo &ei) const { return this->copy(); };
};

}

class Addition : public AbstractOperator
{
public:
    virtual bool matches(std::string candidate) { return candidate == "+"; };
    virtual unsigned int priority() { return 0; }
    virtual CAS::AbstractArithmetic *parse(CAS::AbstractArithmetic *first, CAS::AbstractArithmetic *second) { return new CAS::Addition(first, second); };
};

class Three : public AbstractTerminal
{
public:
    virtual CAS::AbstractArithmetic *parse(std::string candidate) { if (candidate == "3") return new CAS::Number(3); else return 0; };
};

#endif //DEMOMODULES_H
