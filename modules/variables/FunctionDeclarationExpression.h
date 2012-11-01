#ifndef FUNCTIONDECLARATIONEXPRESSION_H
#define FUNCTIONDECLARATIONEXPRESSION_H

#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Scope.h"

class FunctionDeclarationExpression : public CAS::AbstractExpression
{
private:
    CAS::FunctionSignature sig;
    CAS::AbstractExpression::ReturnType returnType;
    std::vector<std::string> argumentNames;
    /*
    std::string id;
    std::vector<CAS::AbstractExpression::ReturnType> argumentTypes;
    std::vector<std::string> argumentNames;
    */

public:
    FunctionDeclarationExpression(CAS::FunctionSignature sig, CAS::AbstractExpression::ReturnType returnType, std::vector<std::string> argumentNames) : sig(std::move(sig)), returnType(returnType), argumentNames(std::move(argumentNames)) {}
    virtual ExpressionP copy() const { return make_unique<FunctionDeclarationExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool) const { scope.declareFunc(sig, returnType); return std::make_pair(UNKNOWN, copy()); }
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const { return typeid(*other) == typeid(FunctionDeclarationExpression) && *this == *static_cast<const FunctionDeclarationExpression*>(other); }

    const CAS::FunctionSignature &getSignature() const { return sig; }
    CAS::AbstractExpression::ReturnType getReturnType() const { return returnType; }
    const std::vector<std::string> &getArgNames() const { return argumentNames; }

    bool operator==(const FunctionDeclarationExpression &other) const { return returnType == other.returnType && sig == other.sig; }
};

#endif // FUNCTIONDECLARATIONEXPRESSION_H
