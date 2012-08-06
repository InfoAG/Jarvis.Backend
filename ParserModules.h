#ifndef PARSERMODULES_H
#define PARSERMODULES_H

#include <QList>
#include <QDataStream>
#include "TerminalModule.h"
#include "OperatorModule.h"
#include "FunctionModule.h"

struct ParserModules
{
    QList<TerminalModule> terminals;
    QList<OperatorModule> operators;
    QList<FunctionModule> functions;

    void removePkg(ModulePackage *pkg);

    ParserModules &operator+=(const ParserModules &other) { terminals += other.terminals; operators += other.operators; functions += other.functions; return *this; }
};

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules);

#endif //PARSERMODULES_H
