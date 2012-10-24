#include "OutputExpression.h"

std::string OutputExpression::toString() const
{
    if (operands.empty()) return std::string();
    std::string result = operands.front()->toString();
    for (auto it = operands.cbegin() + 1; it != operands.cend(); ++it)
        result += '\n' + (*it)->toString();
    return result;
}

bool OutputExpression::equals(const AbstractExpression *other) const
{
    if (typeid(*other) != typeid(OutputExpression)) return false;
    return equalOperands(static_cast<const OutputExpression*>(other)->getOperands(), operands);
}
