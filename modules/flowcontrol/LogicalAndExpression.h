#ifndef LOGICALANDEXPRESSION_H
#define LOGICALANDEXPRESSION_H

#include "Arithmetic/AbstractBinaryOperation.h"
#include "BoolValue.h"

class LogicalAndExpression : public CAS::AbstractBinaryOperation
{
public:
    LogicalAndExpression(ExpressionP first_op, ExpressionP second_op) : AbstractBinaryOperation(std::move(first_op), std::move(second_op)) {}
    virtual ExpressionP copy() const { return make_unique<LogicalAndExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return first_op->toString() + "&&" + second_op->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // LOGICALANDEXPRESSION_H
