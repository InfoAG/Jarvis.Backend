#include "LessExpression.h"

CAS::AbstractExpression::EvalRes LessExpression::eval(CAS::Scope &scope, bool lazy) const
{
    auto firstOpResult = first_op->eval(scope, lazy), secondOpResult = second_op->eval(scope, lazy);
    if (firstOpResult.first != NUMBER || secondOpResult.first != NUMBER) throw "(:";
    else if (typeid(*(firstOpResult.second)) == typeid(CAS::NumberArith) && typeid(*(secondOpResult.second)) == typeid(CAS::NumberArith))
        return std::make_pair(BOOL, make_unique<BoolValue>(static_cast<CAS::NumberArith*>(firstOpResult.second.get())->getValue() < static_cast<CAS::NumberArith*>(secondOpResult.second.get())->getValue()));
    else return std::make_pair(BOOL, make_unique<LessExpression>(std::move(firstOpResult.second), std::move(secondOpResult.second)));
}

bool LessExpression::equals(const CAS::AbstractExpression *other) const
{
    return typeid(*other) == typeid(LessExpression) && first_op->equals(static_cast<const LessExpression*>(other)->getFirstOp().get()) && second_op->equals(static_cast<const LessExpression*>(other)->getFirstOp().get());
}
