#include "VariableDeclarationExpression.h"

bool VariableDeclarationExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(VariableDeclarationExpression) && id == static_cast<const VariableDeclarationExpression*>(other)->getID() && type == static_cast<const VariableDeclarationExpression*>(other)->getType();
}
