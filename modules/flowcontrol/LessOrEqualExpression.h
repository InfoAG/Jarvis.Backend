#ifndef LESSOREQUALEXPRESSION_H
#define LESSOREQUALEXPRESSION_H

#include "Arithmetic/AbstractBinaryOperation.h"
#include "Arithmetic/NumberArith.h"
#include "BoolValue.h"

class LessOrEqualExpression : public CAS::AbstractBinaryOperation
{
public:
    LessOrEqualExpression(ExpressionP first_op, ExpressionP second_op) : AbstractBinaryOperation(std::move(first_op), std::move(second_op)) {}
    virtual ExpressionP copy() const { return make_unique<LessOrEqualExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return first_op->toString() + "<=" + second_op->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // LESSOREQUALEXPRESSION_H
