#ifndef MULTILINEEXPRESSION_H
#define MULTILINEEXPRESSION_H

#include "Arithmetic/AbstractLevelingOperation.h"
#include "Arithmetic/Scope.h"
#include "OutputExpression.h"
#include "ReturnExpression.h"

class MultiLineExpression : public CAS::AbstractLevelingOperation
{
public:
    MultiLineExpression(Operands operands) : AbstractLevelingOperation(std::move(operands)) {}
    virtual std::unique_ptr<AbstractExpression> copy() const { return make_unique<MultiLineExpression>(*this); }

    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const;
};

#endif // MULTILINEEXPRESSION_H
