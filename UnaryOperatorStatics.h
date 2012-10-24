#ifndef UNARYOPERATORSTATICS_H
#define UNARYOPERATORSTATICS_H

#include "OperatorStatics.h"
#include "UnaryOperatorInterface.h"

struct UnaryOperatorStatics : public OperatorStatics {
    UnaryOperatorInterface::Alignment alignment;
};

#endif // UNARYOPERATORSTATICS_H
