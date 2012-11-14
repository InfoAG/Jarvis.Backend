#ifndef OPERATORMODULE_H
#define OPERATORMODULE_H

#include "expression/AbstractExpression.h"
#include <string>
#include "OperatorInterface.h"
#include <QString>
#include "ParserModule.h"
#include <QDataStream>
#include <QTextStream>

template <class InterfaceT, class StaticsT>
class OperatorModule : public ParserModule
{
protected:
    InterfaceT interface;
    StaticsT statics;

public:
    OperatorModule(const QString &name, const ModulePackage *parent_pkg) : ParserModule(name, parent_pkg) {}
    OperatorModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const InterfaceT &interface, const StaticsT &statics) : ParserModule(name, description, parent_pkg), interface(interface), statics(statics) {}

    unsigned int priority() const;
    bool needsParseForMatch() const { return statics.needsParseForMatch; }

    template <class FInterfaceT, class FStaticsT>
    friend QDataStream &operator<<(QDataStream &, const OperatorModule<FInterfaceT, FStaticsT> &);
};

/*
template <class InterfaceT, class StaticsT>
QDataStream &operator<<(QDataStream &stream, const OperatorModule<InterfaceT, StaticsT> &module);
*/

#include "OperatorModule.cpp"

#endif //OPERATORMODULE_H
