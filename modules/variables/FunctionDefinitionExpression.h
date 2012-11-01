#ifndef FUNCTIONDEFINITIONEXPRESSION_H
#define FUNCTIONDEFINITIONEXPRESSION_H

#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Scope.h"
#include "FunctionDeclarationExpression.h"

class FunctionDefinitionExpression : public CAS::AbstractExpression
{
private:
    FunctionDeclarationExpression head;
    ExpressionP body;

public:
    FunctionDefinitionExpression(FunctionDeclarationExpression head, ExpressionP body) : head(std::move(head)), body(std::move(body)) {}
    FunctionDefinitionExpression(const FunctionDefinitionExpression &other) : head(other.head), body(other.body->copy()) {}
    virtual ExpressionP copy() const { return make_unique<FunctionDefinitionExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return head.toString() + " {\n" + body->toString() + "\n}"; }
    virtual bool equals(const AbstractExpression *other) const { return typeid(*other) == typeid(FunctionDefinitionExpression) && *this == *static_cast<const FunctionDefinitionExpression*>(other); }

    bool operator==(const FunctionDefinitionExpression &other) const { return head == other.head && body->equals(other.body.get()); }
};

#endif // FUNCTIONDEFINITIONEXPRESSION_H
