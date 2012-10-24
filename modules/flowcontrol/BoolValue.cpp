#include "BoolValue.h"

bool BoolValue::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(BoolValue) && static_cast<const BoolValue*>(other)->value() == value();
}
