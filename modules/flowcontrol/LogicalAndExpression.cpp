#include "LogicalAndExpression.h"

CAS::AbstractExpression::EvalRes LogicalAndExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != BOOL || secondOpResult.first != BOOL) throw "args derp";
    else if (typeid(*(firstOpResult.second)) == typeid(BoolValue) && typeid(*(secondOpResult.second)) == typeid(BoolValue))
        return std::make_pair(BOOL, make_unique<BoolValue>(static_cast<BoolValue*>(firstOpResult.second.get())->value() && static_cast<BoolValue*>(secondOpResult.second.get())->value()));
    else return std::make_pair(BOOL, make_unique<LogicalAndExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool LogicalAndExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(LogicalAndExpression) && first_op->equals(static_cast<const LogicalAndExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const LogicalAndExpression*>(other)->getFirstOp().get());
}
