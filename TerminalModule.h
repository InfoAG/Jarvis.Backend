#ifndef TERMINALMODULE_H
#define TERMINALMODULE_H

#include "Arithmetic/AbstractArithmetic.h"
#include <string>
#include "ParserModule.h"

class TerminalModule : public ParserModule
{
private:
    std::function<std::unique_ptr<CAS::AbstractArithmetic>(const std::string &, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)>)> parse_interface;

public:
    TerminalModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)>)> parse_interface) : ParserModule(name, description, parent_pkg), parse_interface(parse_interface) {};

    std::unique_ptr<CAS::AbstractArithmetic> parse(const std::string &input, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)> parseFunc) const { return parse_interface(input, parseFunc); }
};

//QDataStream &operator<<(QDataStream &stream, const TerminalModule &module) {

#endif //TERMINALMODULE_H
