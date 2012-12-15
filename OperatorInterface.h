#ifndef OPERATORINTERFACE_H
#define OPERATORINTERFACE_H

#include <functional>
#include <string>
#include <memory>
#include "lang/AbstractStatement.h"

class ExpressionParser;

struct OperatorInterface
{
    std::function<unsigned int()> priority;
};

#endif // OPERATORINTERFACE_H
