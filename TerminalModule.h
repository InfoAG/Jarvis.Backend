#ifndef TERMINALMODULE_H
#define TERMINALMODULE_H

#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include "ParserModule.h"
#include <string>

class TerminalModule : public ParserModule
{
private:
    std::unique_ptr<CAS::AbstractArithmetic> (*parse_interface)(std::string);

public:
    TerminalModule(const ParserModule& parent_pkg, std::unique_ptr<CAS::AbstractArithmetic> (*parse)(std::string)) : ParserModule(parent_pkg), parse_interface(parse) {};

    inline std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input) const { return parse_interface(input); }
};

#endif //TERMINALMODULE_H
