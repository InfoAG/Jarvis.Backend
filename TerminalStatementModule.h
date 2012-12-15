#ifndef TERMINALSTATEMENTMODULE_H
#define TERMINALSTATEMENTMODULE_H

#include "lang/AbstractExpression.h"
#include <string>
#include "ParserModule.h"
#include <QTextStream>

class TerminalStatementModule : public ParserModule
{
private:
    std::function<CAS::AbstractStatement::StatementP(const std::string &, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)> parse_interface;

public:
    TerminalStatementModule(const QString &name, const ModulePackage *parent_pkg) : ParserModule(name, parent_pkg) {}
    TerminalStatementModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, std::function<CAS::AbstractStatement::StatementP(const std::string &, const std::function<CAS::AbstractStatement::StatementP(std::string)> &, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &)> parse_interface) : ParserModule(name, description, parent_pkg), parse_interface(std::move(parse_interface)) {}

    CAS::AbstractStatement::StatementP parse(const std::string &input, const std::function<CAS::AbstractStatement::StatementP(std::string)> &parseStatementFunc, const std::function<CAS::AbstractExpression::ExpressionP(std::string)> &parseExpressionFunc) const { return parse_interface(input, parseStatementFunc, parseExpressionFunc); }

    friend QTextStream &operator>>(QTextStream &stream, TerminalStatementModule &module);
};

#endif // TERMINALSTATEMENTMODULE_H
