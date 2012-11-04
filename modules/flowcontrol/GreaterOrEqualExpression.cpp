#include "GreaterOrEqualExpression.h"

CAS::AbstractExpression::EvalRes GreaterOrEqualExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != CAS::TypeInfo::NUMBER || secondOpResult.first != CAS::TypeInfo::NUMBER) throw "(:";
    else if (typeid(*(firstOpResult.second)) == typeid(CAS::NumberArith) && typeid(*(secondOpResult.second)) == typeid(CAS::NumberArith))
        return std::make_pair(CAS::TypeInfo::BOOL, make_unique<BoolValue>(static_cast<CAS::NumberArith*>(firstOpResult.second.get())->getValue() < static_cast<CAS::NumberArith*>(secondOpResult.second.get())->getValue()));
    else return std::make_pair(CAS::TypeInfo::BOOL, make_unique<GreaterOrEqualExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool GreaterOrEqualExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(GreaterOrEqualExpression) && first_op->equals(static_cast<const GreaterOrEqualExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const GreaterOrEqualExpression*>(other)->getFirstOp().get());
}
