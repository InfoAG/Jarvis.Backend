#ifndef EMPTYEXPRESSION_H
#define EMPTYEXPRESSION_H

#include "Arithmetic/AbstractExpression.h"

class EmptyExpression : public CAS::AbstractExpression
{
public:
    virtual ExpressionP copy() const { return make_unique<EmptyExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &, bool) const { return std::make_pair(UNKNOWN, copy()); }
    virtual std::string toString() const { return std::string(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // EMPTYEXPRESSION_H
