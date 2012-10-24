#ifndef PARSERMODULES_H
#define PARSERMODULES_H

#include <QList>
#include <QDataStream>
#include "TerminalModule.h"
#include "BinaryOperatorModule.h"
#include "UnaryOperatorModule.h"
#include "FunctionModule.h"

struct ParserModules
{
    QList<TerminalModule> terminals;
    QList<BinaryOperatorModule> binaryOperators;
    QList<UnaryOperatorModule> unaryOperators;
    QList<FunctionModule> functions;

    void removePkg(ModulePackage *pkg);

    ParserModules &operator+=(const ParserModules &other) { terminals += other.terminals; binaryOperators += other.binaryOperators; unaryOperators += other.unaryOperators; functions += other.functions; return *this; }
};

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules);

#endif //PARSERMODULES_H
