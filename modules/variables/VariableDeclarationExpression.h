#ifndef VARIABLEDECLARATIONEXPRESSION_H
#define VARIABLEDECLARATIONEXPRESSION_H

#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Scope.h"

class VariableDeclarationExpression : public CAS::AbstractExpression
{
private:
    CAS::AbstractExpression::ReturnType type;
    std::string id;

public:
    VariableDeclarationExpression(CAS::AbstractExpression::ReturnType type, std::string id) : type(type), id(std::move(id)) {}
    virtual ExpressionP copy() const { return make_unique<VariableDeclarationExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool) const { scope.declareVar(type, id); return std::make_pair(UNKNOWN, copy()); }
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const;

    std::string getID() const { return id; }
    CAS::AbstractExpression::ReturnType getType() const { return type; }
};

#endif // VARIABLEDECLARATIONEXPRESSION_H
