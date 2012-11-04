#include "LogicalOrExpression.h"

CAS::AbstractExpression::EvalRes LogicalOrExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != CAS::TypeInfo::BOOL || secondOpResult.first != CAS::TypeInfo::BOOL) throw "args derp";
    else if (typeid(*(firstOpResult.second)) == typeid(BoolValue) && typeid(*(secondOpResult.second)) == typeid(BoolValue))
        return std::make_pair(CAS::TypeInfo::BOOL, make_unique<BoolValue>(static_cast<BoolValue*>(firstOpResult.second.get())->value() || static_cast<BoolValue*>(secondOpResult.second.get())->value()));
    else return std::make_pair(CAS::TypeInfo::BOOL, make_unique<LogicalOrExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool LogicalOrExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(LogicalOrExpression) && first_op->equals(static_cast<const LogicalOrExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const LogicalOrExpression*>(other)->getFirstOp().get());
}
