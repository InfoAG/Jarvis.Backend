#ifndef SCOPEEXPRESSION_H
#define SCOPEEXPRESSION_H

#include "Arithmetic/AbstractLevelingOperation.h"
#include "Arithmetic/Scope.h"
#include "OutputExpression.h"

class ScopeExpression : public CAS::AbstractLevelingOperation
{
public:
    ScopeExpression(Operands operands) : AbstractLevelingOperation(std::move(operands)) {}
    virtual std::unique_ptr<AbstractExpression> copy() const { return make_unique<ScopeExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // SCOPEEXPRESSION_H
