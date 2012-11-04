#ifndef BOOLVALUE_H
#define BOOLVALUE_H

#include "Arithmetic/AbstractExpression.h"

class BoolValue : public CAS::AbstractExpression
{
private:
    bool value_;

public:
    BoolValue(bool value) : value_(value) {}

    virtual ExpressionP copy() const { return make_unique<BoolValue>(*this); }
    virtual EvalRes eval(CAS::Scope &, bool) const { return std::make_pair(CAS::TypeInfo::BOOL, copy()); }
    virtual std::string toString() const { return value_ ? "true" : "false"; }
    virtual bool equals(const AbstractExpression *other) const;

    bool value() const { return value_; }
};

#endif // BOOLVALUE_H
