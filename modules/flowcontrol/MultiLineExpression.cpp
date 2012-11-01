#include "MultiLineExpression.h"

CAS::AbstractExpression::EvalRes MultiLineExpression::eval(CAS::Scope &scope, bool lazy) const
{
    Operands result;
    for (const auto &op : operands) {
        auto evalRes = op->eval(scope, lazy);
        if (typeid(*(evalRes.second)) == typeid(OutputExpression)) result.emplace_back(std::move(evalRes.second));
        else if (typeid(*(evalRes.second)) == typeid(ReturnExpression)) return std::make_pair(evalRes.first, std::move(static_cast<ReturnExpression*>(evalRes.second.get())->getOperand()));
    }
    return std::make_pair(UNKNOWN, make_unique<MultiLineExpression>(std::move(result)));
}

std::string MultiLineExpression::toString() const
{
    if (operands.empty()) return std::string();
    std::string result;
    for (auto it = operands.cbegin(); it != operands.cend(); ++it) {
        result += (*it)->toString();
        if (typeid(**it) != typeid(OutputExpression)) result += ";";
        if (it != operands.cend() - 1) result += "\n";
    }
    return result;
}

bool MultiLineExpression::equals(const AbstractExpression *other) const
{
    return typeid(*other) == typeid(MultiLineExpression) && equalOperands(operands, static_cast<const MultiLineExpression*>(other)->getOperands());
}
