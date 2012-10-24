#include "IfExpression.h"

IfExpression::IfExpression(const IfExpression &other)
{
    for (const auto &conditional : other.conditionals)
        conditionals.emplace_back(conditional.first->copy(), conditional.second->copy());
}

CAS::AbstractExpression::EvalRes IfExpression::eval(CAS::Scope &scope, bool lazy) const
{
    std::vector<std::pair<ExpressionP, ExpressionP>> evalRes;
    auto it = conditionals.cbegin();
    for (; it != conditionals.cend(); ++it) {
        auto conditionResult = it->first->eval(scope, lazy);
        if (conditionResult.first != BOOL) throw "wrong argument";
        else if (typeid(*(conditionResult.second)) == typeid(BoolValue)) {
            if (static_cast<BoolValue*>(conditionResult.second.get())->value())
                return it->second->eval(scope, lazy);
        } else {
            evalRes.emplace_back(std::move(conditionResult.second), it->second->copy());
            ++it;
            break;
        }
    }

    if (it != conditionals.cend()) {
        for (; it != conditionals.cend(); ++it) {
            auto conditionResult = it->first->eval(scope, lazy);
            if (conditionResult.first != BOOL) throw "wrong argument";
            evalRes.emplace_back(std::move(conditionResult.second), it->second->copy());
        }
    }
    if (evalRes.empty()) return std::make_pair(UNKNOWN, make_unique<OutputExpression>());
    else return std::make_pair(UNKNOWN, make_unique<IfExpression>(std::move(evalRes)));
}

std::string IfExpression::toString() const
{
    std::string result = "if ";
    for (auto it = conditionals.cbegin(); it != conditionals.cend(); ++it) {
        if (it != conditionals.cbegin()) result += " else if ";
        result += "(" + it->first->toString() + ") {\n" + it->second->toString() + "\n}";
    }
    return result;
}

bool IfExpression::equals(const CAS::AbstractExpression *other) const
{
    if (typeid(*other) != typeid(IfExpression) || static_cast<const IfExpression*>(other)->getConditionals().size() != conditionals.size()) return false;
    else {
        auto itOther = static_cast<const IfExpression*>(other)->getConditionals().cbegin();
        for (const auto &conditional : conditionals) {
            if (! (conditional.first->equals(itOther->first.get()) && conditional.second->equals(itOther->second.get()))) return false;
            ++itOther;
        }
        return true;
    }
}
