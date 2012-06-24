#ifndef PARSER_H
#define PARSER_H

#include "../Framework/Container/SmartList.h"
#include <string>
#include <map>
#include "AbstractFunction.h"
#include "AbstractOperator.h"
#include "AbstractTerminal.h"

class Parser
{
private:
    CAS::SmartList<AbstractTerminal*> terminals;
    CAS::SmartList<AbstractOperator*> operators;
    CAS::SmartList<AbstractFunction*> functions;

public:
    Parser();

    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
