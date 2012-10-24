#include "NegationExpression.h"

CAS::AbstractExpression::EvalRes NegationExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto operandRes = operand->eval(scope, lazy);
    if (operandRes.first != BOOL) throw "type derp";
    else if (typeid(*(operandRes.second)) == typeid(BoolValue))
        return std::make_pair(BOOL, make_unique<BoolValue>(! static_cast<BoolValue*>(operandRes.second.get())->value()));
    else return std::make_pair(BOOL, make_unique<NegationExpression>(std::move(operandRes.second)));
}

bool NegationExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(NegationExpression) && operand->equals(static_cast<const NegationExpression*>(other)->getOperand().get());
}
