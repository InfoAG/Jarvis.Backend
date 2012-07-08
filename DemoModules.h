#ifndef DEMOMODULES_H
#define DEMOMODULES_H
/*
#include "AbstractOperator.h"
#include "AbstractTerminal.h"
#include "../Framework/Arithmetic/Addition.h"
#include <cstdlib>

class AdditionExpression : public AbstractOperator
{
public:
    virtual bool matches(std::string candidate) const { return candidate == "+"; };
    virtual unsigned int priority() const { return 0; }
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) const { return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Addition(std::move(first), std::move(second))); };
};

class NumberExpression : public AbstractTerminal
{
private:
    enum state {START, DEAD, ACCPT,		 ZFIRST			, NFIRST					, FIRSTT								, VORZ		     , VORZNT};
    //			q0	 , tot , akzeptiert, Null am Anfang , Zahl auﬂer Null am Anfang , erstes Trennzeichen (Komma oder Bruch), Minus am Anfang, Minus nach Trennzeichen

    state delta(state stt, char str) const;

public:
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::string candidate) const;
};
*/
#endif //DEMOMODULES_H
