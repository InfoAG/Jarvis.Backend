#include "RangedForExpression.h"

CAS::AbstractExpression::EvalRes RangedForExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto listRes = list->eval(scope, lazy);
    if (((typeid(*declaration) != typeid(VariableDeclarationExpression) || static_cast<VariableDeclarationExpression*>(declaration.get())->getIDs().size() != 1) && typeid(*declaration) != typeid(CAS::Variable)) || listRes.first != CAS::TypeInfo::LIST) throw "herp";
    else if (typeid(*(listRes.second)) == typeid(CAS::List)) {
        std::string varID;
        if (typeid(*declaration) == typeid(CAS::Variable))
            varID = static_cast<CAS::Variable*>(declaration.get())->getIdentifier();
        else varID = static_cast<VariableDeclarationExpression*>(declaration.get())->getIDs().front();
        CAS::Scope forScope(&scope);
        declaration->eval(forScope, lazy);
        Operands result;
        for (const auto &item : static_cast<CAS::List*>(listRes.second.get())->getOperands()) {
            forScope.defineVar(varID, item->eval(scope, lazy));
            result.emplace_back(make_unique<OutputExpression>(instruction->eval(forScope, lazy).second));
        }
        return std::make_pair(CAS::TypeInfo::VOID, make_unique<MultiLineExpression>(std::move(result)));
    } else return std::make_pair(CAS::TypeInfo::VOID, make_unique<RangedForExpression>(declaration->copy(), std::move(listRes.second), instruction->copy()));
}
