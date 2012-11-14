#ifndef TERMINALMODULE_H
#define TERMINALMODULE_H

#include "expression/AbstractExpression.h"
#include <string>
#include "ParserModule.h"
#include <QTextStream>

class TerminalModule : public ParserModule
{
private:
    std::function<std::unique_ptr<CAS::AbstractExpression>(const std::string &, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)> parse_interface;

public:
    TerminalModule(const QString &name, const ModulePackage *parent_pkg) : ParserModule(name, parent_pkg) {}
    TerminalModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)> parse_interface) : ParserModule(name, description, parent_pkg), parse_interface(parse_interface) {};

    std::unique_ptr<CAS::AbstractExpression> parse(const std::string &input, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)> parseFunc) const { return parse_interface(input, parseFunc); }

    friend QTextStream &operator>>(QTextStream &stream, TerminalModule &module);
};


#endif //TERMINALMODULE_H
