#include "ScopeExpression.h"

CAS::AbstractExpression::EvalRes ScopeExpression::eval(CAS::Scope &scope, bool lazy) const
{
    CAS::Scope myScope(&scope);
    auto opRes = operand->eval(myScope, lazy);
    return std::make_pair(opRes.first, std::move(opRes.second));
}

bool ScopeExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(ScopeExpression) && operand->equals(static_cast<const ScopeExpression*>(other)->getOperand().get());
}
