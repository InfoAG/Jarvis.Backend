#include "OutputExpression.h"

bool OutputExpression::equals(const AbstractExpression *other) const
{
    return typeid(*other) == typeid(OutputExpression) && operand->equals(static_cast<const OutputExpression*>(other)->getOperand().get());
}
