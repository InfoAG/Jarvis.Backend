#ifndef BINARYOPERATORINTERFACE_H
#define BINARYOPERATORINTERFACE_H

#include "OperatorInterface.h"

struct BinaryOperatorInterface : public OperatorInterface {
    enum AssociativityType { LEFT, RIGHT };

    std::function<std::pair<bool, size_t>(const std::string&, size_t, const ExpressionParser&)> matches;
    std::function<AssociativityType()> associativity;
    std::function<std::unique_ptr<CAS::AbstractExpression>(std::unique_ptr<CAS::AbstractExpression>, std::unique_ptr<CAS::AbstractExpression>)> parse;
};

#endif // BINARYOPERATORINTERFACE_H
