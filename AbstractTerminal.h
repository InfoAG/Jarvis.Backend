#ifndef ABSTRACTTERMINAL_H
#define ABSTRACTTERMINAL_H

#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include <string>

class AbstractTerminal
{
public:
    virtual std::unique_ptr<CAS::AbstractArithmetic> parse(std::string) = 0;
};

#endif //ABSTRACTTERMINAL_H
