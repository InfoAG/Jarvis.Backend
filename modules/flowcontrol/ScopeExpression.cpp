#include "ScopeExpression.h"

CAS::AbstractExpression::EvalRes ScopeExpression::eval(CAS::Scope &scope, bool lazy) const
{
    CAS::Scope myScope(&scope);
    Operands result;
    for (const auto &op : operands) {
        auto evalRes = op->eval(myScope, lazy).second;
        if (typeid(*evalRes) == typeid(OutputExpression)) result.emplace_back(std::move(evalRes));
    }
    return std::make_pair(UNKNOWN, make_unique<OutputExpression>(std::move(result)));
}

std::string ScopeExpression::toString() const
{
    std::string result = "{";
    for (auto it = operands.cbegin(); it != operands.cend() - 1; ++it)
        result += (*it)->toString() + "\n";
    result += operands.back()->toString();
    return result + "}";
}

bool ScopeExpression::equals(const AbstractExpression *other) const
{
    return typeid(*other) == typeid(ScopeExpression) && equalOperands(operands, static_cast<const ScopeExpression*>(other)->getOperands());
}
