#include "VariableDeclarationExpression.h"

std::string VariableDeclarationExpression::toString() const
{
    std::string result;
    switch (type) {
    case NUMBER: result = "number"; break;
    case BOOL: result = "bool"; break;
    case LIST: result = "list"; break;
    }
    return result + " " + id;
}

bool VariableDeclarationExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(VariableDeclarationExpression) && id == static_cast<const VariableDeclarationExpression*>(other)->getID() && type == static_cast<const VariableDeclarationExpression*>(other)->getType();
}
