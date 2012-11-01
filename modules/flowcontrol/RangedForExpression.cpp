#include "RangedForExpression.h"

CAS::AbstractExpression::EvalRes RangedForExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto listRes = list->eval(scope, lazy);
    if ((typeid(*declaration) != typeid(VariableDeclarationExpression) && typeid(*declaration) != typeid(CAS::Variable)) || listRes.first != LIST) throw "herp";
    else if (typeid(*(listRes.second)) == typeid(CAS::List)) {
        std::string varID;
        if (typeid(*declaration) == typeid(CAS::Variable))
            varID = static_cast<CAS::Variable*>(declaration.get())->getIdentifier();
        else varID = static_cast<VariableDeclarationExpression*>(declaration.get())->getID();
        CAS::Scope forScope(&scope);
        declaration->eval(forScope, lazy);
        Operands result;
        for (const auto &item : static_cast<CAS::List*>(listRes.second.get())->getOperands()) {
            forScope.defineVar(varID, item->eval(scope, lazy));
            result.emplace_back(instruction->eval(forScope, lazy).second);
        }
        return std::make_pair(UNKNOWN, make_unique<OutputExpression>(std::move(result)));
    } else return std::make_pair(UNKNOWN, make_unique<RangedForExpression>(declaration->copy(), std::move(listRes.second), instruction->copy()));
}
