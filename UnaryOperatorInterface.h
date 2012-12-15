#ifndef UNARYOPERATORINTERFACE_H
#define UNARYOPERATORINTERFACE_H

#include "OperatorInterface.h"
#include "lang/AbstractExpression.h"

struct UnaryOperatorInterface : public OperatorInterface
{
    enum Alignment { PRE, POST };

    std::function<std::pair<bool, size_t>(const std::string&)> matches;
    std::function<Alignment()> alignment;
    std::function<CAS::AbstractExpression::ExpressionP(CAS::AbstractExpression::ExpressionP)> parse;
};

#endif // UNARYOPERATORINTERFACE_H
