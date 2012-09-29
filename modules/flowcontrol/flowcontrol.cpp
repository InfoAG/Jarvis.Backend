#include "flowcontrol_global.h"
#include "ExpressionParser.h"
#include "Arithmetic/ScopeExpression.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> FLOWCONTROLSHARED_EXPORT Scope_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc)
{
    if (candidate.front() != '{' || candidate.back() != '}') return nullptr;
    int level = 0;
    for (auto it = candidate.begin() + 1; it != candidate.end() - 1; ++it) {
        if (*it == '{') level++;
        else if (*it == '}' && --level == -1) return nullptr;
    }

    CAS::AbstractExpression::Operands selectTokens;
    std::unique_ptr<CAS::AbstractExpression> tmpToken;
    for (const auto &token : ExpressionParser::tokenize({candidate.cbegin() + 1, candidate.cend() - 1}, ";\n"))
        if (tmpToken = parseFunc(token)) selectTokens.emplace_back(std::move(tmpToken));
    return make_unique<CAS::ScopeExpression>(std::move(selectTokens));
}

}
