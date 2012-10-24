#include "EmptyExpression.h"

bool EmptyExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(EmptyExpression);
}
