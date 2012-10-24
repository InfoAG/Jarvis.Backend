#ifndef UNARYOPERATORINTERFACE_H
#define UNARYOPERATORINTERFACE_H

#include "OperatorInterface.h"

struct UnaryOperatorInterface : public OperatorInterface
{
    enum Alignment { PRE, POST };

    std::function<std::pair<bool, size_t>(const std::string&)> matches;
    std::function<Alignment()> alignment;
    std::function<std::unique_ptr<CAS::AbstractExpression>(std::unique_ptr<CAS::AbstractExpression>)> parse;
};

#endif // UNARYOPERATORINTERFACE_H
