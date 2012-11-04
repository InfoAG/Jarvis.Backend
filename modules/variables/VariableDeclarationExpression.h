#ifndef VARIABLEDECLARATIONEXPRESSION_H
#define VARIABLEDECLARATIONEXPRESSION_H

#include "Arithmetic/AbstractLevelingOperation.h"
#include "Arithmetic/Scope.h"

class VariableDeclarationExpression : public CAS::AbstractLevelingOperation
{
private:
    CAS::TypeInfo type;
    std::vector<std::string> ids;

public:
    VariableDeclarationExpression(CAS::TypeInfo type, std::vector<std::string> ids) : type(std::move(type)), ids(std::move(ids)) {}
    virtual ExpressionP copy() const { return make_unique<VariableDeclarationExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool) const;
    virtual std::string toString() const;
    virtual bool equals(const AbstractExpression *other) const { return typeid(*other) == typeid(VariableDeclarationExpression) && *this == *static_cast<const VariableDeclarationExpression*>(other); }

    CAS::TypeInfo getType() const { return type; }
    const std::vector<std::string> &getIDs() const { return ids; }

    bool operator==(const VariableDeclarationExpression &other) const { return type == other.type && ids == other.ids; }
};

#endif // VARIABLEDECLARATIONEXPRESSION_H
