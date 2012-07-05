#ifndef PARSERMODULES_H
#define PARSERMODULES_H

#include "AbstractFunction.h"
#include "AbstractOperator.h"
#include "AbstractTerminal.h"
#include "../Framework/Container/LinkedList.h"

class ParserModules
{
    CAS::LinkedList<std::unique_ptr<AbstractTerminal> > terminals;
    CAS::LinkedList<std::unique_ptr<AbstractOperator> > operators;
    CAS::LinkedList<std::unique_ptr<AbstractFunction> > functions;
public:
    ParserModules() {};
};

#endif //PARSERMODULES_H
