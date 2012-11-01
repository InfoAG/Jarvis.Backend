#include "FunctionDeclarationExpression.h"

std::string FunctionDeclarationExpression::toString() const
{
    std::string result = typeToString(returnType) + " " + sig.id + "(";
    if (! sig.argumentTypes.empty()) {
        for (auto it = sig.argumentTypes.cbegin(); it != sig.argumentTypes.cend() - 1; ++it)
            result += typeToString(*it) + ", ";
        result += typeToString(sig.argumentTypes.back());
    }
    return result + ")";
}
