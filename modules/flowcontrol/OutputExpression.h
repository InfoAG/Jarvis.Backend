#ifndef OUTPUTEXPRESSION_H
#define OUTPUTEXPRESSION_H

#include "Arithmetic/AbstractUnaryOperation.h"

class OutputExpression : public CAS::AbstractUnaryOperation
{
public:
    OutputExpression() {};
    OutputExpression(ExpressionP operand) : AbstractUnaryOperation(std::move(operand)) {}
    virtual ExpressionP copy() const { return make_unique<OutputExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool  lazy) const { return std::make_pair(UNKNOWN, make_unique<OutputExpression>(operand->eval(scope, lazy).second)); }
    virtual std::string toString() const { if (operand == nullptr) return std::string(); else return operand->toString(); }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // OUTPUTEXPRESSION_H
