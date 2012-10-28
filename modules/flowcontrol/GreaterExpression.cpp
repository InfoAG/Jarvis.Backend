#include "GreaterExpression.h"

CAS::AbstractExpression::EvalRes GreaterExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != NUMBER || secondOpResult.first != NUMBER) throw "(:";
    else if (typeid(*(firstOpResult.second)) == typeid(CAS::NumberArith) && typeid(*(secondOpResult.second)) == typeid(CAS::NumberArith))
        return std::make_pair(BOOL, make_unique<BoolValue>(static_cast<CAS::NumberArith*>(firstOpResult.second.get())->getValue() < static_cast<CAS::NumberArith*>(secondOpResult.second.get())->getValue()));
    else return std::make_pair(BOOL, make_unique<GreaterExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool GreaterExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(GreaterExpression) && first_op->equals(static_cast<const GreaterExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const GreaterExpression*>(other)->getFirstOp().get());
}
