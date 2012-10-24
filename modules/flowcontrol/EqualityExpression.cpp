#include "EqualityExpression.h"

CAS::AbstractExpression::EvalRes EqualityExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != secondOpResult.first) return std::make_pair(BOOL, make_unique<BoolValue>(false));
    else if (typeid(*(firstOpResult.second)) == typeid(BoolValue) && typeid(*(secondOpResult.second)) == typeid(BoolValue))
        return std::make_pair(BOOL, make_unique<BoolValue>(static_cast<BoolValue*>(firstOpResult.second.get())->value() == static_cast<BoolValue*>(secondOpResult.second.get())->value()));
    else if (typeid(*(firstOpResult.second)) == typeid(CAS::NumberArith) && typeid(*(secondOpResult.second)) == typeid(CAS::NumberArith))
        return std::make_pair(BOOL, make_unique<BoolValue>(static_cast<CAS::NumberArith*>(firstOpResult.second.get())->getValue() == static_cast<CAS::NumberArith*>(secondOpResult.second.get())->getValue()));
    else if (firstOpResult.second->equals(secondOpResult.second.get()))
        return std::make_pair(BOOL, make_unique<BoolValue>(true));         
    else return std::make_pair(BOOL, make_unique<EqualityExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool EqualityExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(EqualityExpression) && first_op->equals(static_cast<const EqualityExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const EqualityExpression*>(other)->getFirstOp().get());
}
