#ifndef TERMINALEXPRESSIONMODULE_H
#define TERMINALEXPRESSIONMODULE_H

#include "lang/AbstractExpression.h"
#include <string>
#include "ParserModule.h"
#include <QTextStream>

class TerminalExpressionModule : public ParserModule
{
private:
    std::function<CAS::AbstractExpression::ExpressionP(const std::string &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)> parse_interface;

public:
    TerminalExpressionModule(const QString &name, const ModulePackage *parent_pkg) : ParserModule(name, parent_pkg) {}
    TerminalExpressionModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, std::function<CAS::AbstractExpression::ExpressionP(const std::string &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)> parse_interface) : ParserModule(name, description, parent_pkg), parse_interface(std::move(parse_interface)) {}

    CAS::AbstractExpression::ExpressionP parse(const std::string &input, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc) const { return parse_interface(input, parseExpressionFunc); }

    friend QTextStream &operator>>(QTextStream &stream, TerminalExpressionModule &module);
};


#endif //TERMINALEXPRESSIONMODULE_H
