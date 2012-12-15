#ifndef PARSERMODULES_H
#define PARSERMODULES_H

#include <QList>
#include <QDataStream>
#include "TerminalExpressionModule.h"
#include "TerminalStatementModule.h"
#include "BinaryOperatorModule.h"
#include "UnaryOperatorModule.h"
#include "FunctionModule.h"

struct ParserModules
{
    QList<std::shared_ptr<TerminalStatementModule>> terminalStatements;
    QList<std::shared_ptr<TerminalExpressionModule>> terminalExpressions;
    QList<std::shared_ptr<BinaryOperatorModule>> binaryOperators;
    QList<std::shared_ptr<UnaryOperatorModule>> unaryOperators;
    QList<std::shared_ptr<FunctionModule>> functions;

    void removePkg(ModulePackage *pkg);

    ParserModules &operator+=(const ParserModules &other) { terminalStatements += other.terminalStatements; terminalExpressions += other.terminalExpressions; binaryOperators += other.binaryOperators; unaryOperators += other.unaryOperators; functions += other.functions; return *this; }
};

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules);

#endif //PARSERMODULES_H
