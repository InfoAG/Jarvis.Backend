#ifndef PARSERMODULES_H
#define PARSERMODULES_H

#include "FunctionModule.h"
#include "OperatorModule.h"
#include "TerminalModule.h"
#include "../Framework/Container/SmartList.h"

struct ParserModules
{
    CAS::SmartList<TerminalModule> terminals;
    CAS::SmartList<OperatorModule> operators;
    CAS::SmartList<FunctionModule> functions;

    ParserModules &operator+=(const ParserModules &other) { terminals += other.terminals; operators += other.operators; functions += other.functions; return *this; };
};

#endif //PARSERMODULES_H
