#ifndef LOGICALOREXPRESSION_H
#define LOGICALOREXPRESSION_H

#include "Arithmetic/AbstractBinaryOperation.h"
#include "BoolValue.h"

class LogicalOrExpression : public CAS::AbstractBinaryOperation
{
public:
    LogicalOrExpression(ExpressionP first_op, ExpressionP second_op) : AbstractBinaryOperation(std::move(first_op), std::move(second_op)) {}
    virtual ExpressionP copy() const { return make_unique<LogicalOrExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return first_op->toString() + "&&" + second_op->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // LOGICALOREXPRESSION_H
