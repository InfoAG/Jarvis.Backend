#ifndef EQUALITYEXPRESSION_H
#define EQUALITYEXPRESSION_H

#include "Arithmetic/AbstractBinaryOperation.h"
#include "BoolValue.h"
#include "Arithmetic/NumberArith.h"

class EqualityExpression : public CAS::AbstractBinaryOperation
{
public:
    EqualityExpression(ExpressionP first_op, ExpressionP second_op) : AbstractBinaryOperation(std::move(first_op), std::move(second_op)) {}
    virtual ExpressionP copy() const { return make_unique<EqualityExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return first_op->toString() + "==" + second_op->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // EQUALITYEXPRESSION_H
