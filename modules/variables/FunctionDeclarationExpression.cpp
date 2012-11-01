#include "FunctionDeclarationExpression.h"

std::string FunctionDeclarationExpression::toString() const
{
    std::string result = typeToString(returnType) + " " + id + "(";
    if (! argumentTypes.empty()) {
        for (auto it = argumentTypes.cbegin(); it != argumentTypes.cend() - 1; ++it)
            result += typeToString(*it) + ", ";
        result += typeToString(argumentTypes.back());
    }
    return result + ")";
}
