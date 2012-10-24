#ifndef NEGATIONEXPRESSION_H
#define NEGATIONEXPRESSION_H

#include "Arithmetic/AbstractUnaryOperation.h"
#include "BoolValue.h"

class NegationExpression : public CAS::AbstractUnaryOperation
{
public:
    NegationExpression(ExpressionP operand) : AbstractUnaryOperation(std::move(operand)) {}
    virtual ExpressionP copy() const { return make_unique<NegationExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return "!" + operand->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // NEGATIONEXPRESSION_H
