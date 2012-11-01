#ifndef RETURNEXPRESSION_H
#define RETURNEXPRESSION_H

#include "Arithmetic/AbstractUnaryOperation.h"

class ReturnExpression : public CAS::AbstractUnaryOperation
{
public:
    ReturnExpression(ExpressionP operand) : AbstractUnaryOperation(std::move(operand)) {}
    virtual std::unique_ptr<AbstractExpression> copy() const { return make_unique<ReturnExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return "return " + operand->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // RETURNEXPRESSION_H
