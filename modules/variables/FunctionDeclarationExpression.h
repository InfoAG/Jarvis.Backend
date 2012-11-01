#ifndef FUNCTIONDECLARATIONEXPRESSION_H
#define FUNCTIONDECLARATIONEXPRESSION_H

#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Scope.h"

class FunctionDeclarationExpression : public CAS::AbstractExpression
{
private:
    CAS::AbstractExpression::ReturnType returnType;
    std::string id;
    std::vector<CAS::AbstractExpression::ReturnType> argumentTypes;
    std::vector<std::string> argumentNames;

public:
    FunctionDeclarationExpression(CAS::AbstractExpression::ReturnType returnType, std::string id, std::vector<CAS::AbstractExpression::ReturnType> argumentTypes, std::vector<std::string> argumentNames) : returnType(returnType), id(std::move(id)), argumentTypes(std::move(argumentTypes)), argumentNames(std::move(argumentNames)) {}
    virtual ExpressionP copy() const { return make_unique<FunctionDeclarationExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool) const { scope.declareFunc({id, argumentTypes}, returnType); return std::make_pair(UNKNOWN, copy()); }
    virtual std::string toString() const;
virtual bool equals(const AbstractExpression *other) const { return typeid(*other) == typeid(FunctionDeclarationExpression) && *this == *static_cast<const FunctionDeclarationExpression*>(other); }

    bool operator==(const FunctionDeclarationExpression &other) const { return returnType == other.returnType && id == other.id && argumentTypes == other.argumentTypes; }
};

#endif // FUNCTIONDECLARATIONEXPRESSION_H
