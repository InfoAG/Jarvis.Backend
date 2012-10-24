#ifndef OUTPUTEXPRESSION_H
#define OUTPUTEXPRESSION_H

#include "Arithmetic/AbstractLevelingOperation.h"

class OutputExpression : public CAS::AbstractLevelingOperation
{
public:
    OutputExpression() {}
    OutputExpression(CAS::AbstractExpression::ExpressionP operand) { operands.emplace_back(std::move(operand)); }
    OutputExpression(Operands operands) : AbstractLevelingOperation(std::move(operands)) {}
    virtual ExpressionP copy() const { return make_unique<OutputExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &, bool) const { return std::make_pair(UNKNOWN, copy()); }
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // OUTPUTEXPRESSION_H
