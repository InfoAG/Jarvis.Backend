#ifndef SCOPEEXPRESSION_H
#define SCOPEEXPRESSION_H

#include "Arithmetic/AbstractUnaryOperation.h"
#include "Arithmetic/Scope.h"
#include "OutputExpression.h"

class ScopeExpression : public CAS::AbstractUnaryOperation
{
public:
    ScopeExpression(ExpressionP operand) : AbstractUnaryOperation(std::move(operand)) {}
    virtual std::unique_ptr<AbstractExpression> copy() const { return make_unique<ScopeExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return "{" + operand->toString() + "}"; }
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // SCOPEEXPRESSION_H
