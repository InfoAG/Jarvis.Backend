#ifndef RANGEDFOREXPRESSION_H
#define RANGEDFOREXPRESSION_H

#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Scope.h"
#include "../variables/VariableDeclarationExpression.h"
#include "Arithmetic/List.h"
#include "OutputExpression.h"
#include "Arithmetic/Variable.h"
#include "MultiLineExpression.h"

class RangedForExpression : public CAS::AbstractExpression
{
private:
    ExpressionP declaration;
    ExpressionP list;
    ExpressionP instruction;

public:
    RangedForExpression(ExpressionP declaration, ExpressionP list, ExpressionP instruction) : declaration(std::move(declaration)), list(std::move(list)), instruction(std::move(instruction)) {}
    RangedForExpression(const RangedForExpression &other) : declaration(other.declaration->copy()), list(other.list->copy()), instruction(other.instruction->copy()) {}

    virtual ExpressionP copy() const { return make_unique<RangedForExpression>(*this); }
    virtual EvalRes eval(CAS::Scope &scope, bool lazy) const;
    virtual std::string toString() const { return "for (" + declaration->toString() + " : " + list->toString() + ")\n" + instruction->toString(); }
    virtual bool equals(const AbstractExpression *other) const { return typeid(*other) == typeid(RangedForExpression) && *this == *static_cast<const RangedForExpression*>(other); }

    const ExpressionP &getDeclaration() const { return declaration; }
    const ExpressionP &getList() const { return list; }
    const ExpressionP &getInstruction() const { return instruction; }

    bool operator==(const RangedForExpression &other) const { return declaration->equals(other.declaration.get()) && list->equals(other.list.get()) && instruction->equals(other.instruction.get()); }
};

#endif // RANGEDFOREXPRESSION_H
