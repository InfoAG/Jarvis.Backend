#ifndef BINARYOPERATORSTATICS_H
#define BINARYOPERATORSTATICS_H

#include "OperatorStatics.h"
#include "BinaryOperatorInterface.h"

struct BinaryOperatorStatics : public OperatorStatics {
    BinaryOperatorInterface::AssociativityType associativity;
};

#endif // BINARYOPERATORSTATICS_H
