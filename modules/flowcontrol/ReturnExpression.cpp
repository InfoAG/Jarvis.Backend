#include "ReturnExpression.h"

CAS::AbstractExpression::EvalRes ReturnExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto opRes = operand->eval(scope, lazy);
    if (opRes.first == UNKNOWN) throw "return type";
    return std::make_pair(opRes.first, make_unique<ReturnExpression>(std::move(opRes.second)));
}

bool ReturnExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(ReturnExpression) && operand->equals(static_cast<const ReturnExpression*>(other)->getOperand().get());
}
