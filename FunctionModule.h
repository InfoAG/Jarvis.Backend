#ifndef FUNCTIONMODULE_H
#define FUNCTIONMODULE_H

#include "expression/AbstractExpression.h"
#include <string>
#include "FunctionInterface.h"
#include <QString>
#include "ParserModule.h"
#include <QDataStream>
#include <QPair>
#include <QTextStream>
#include <QStringList>

class FunctionModule : public ParserModule
{
public:
    struct StaticInfo {
        std::shared_ptr<QPair<QString, unsigned int>> matches;
        unsigned int priority;
    };

private:
    FunctionInterface interface;
    StaticInfo statics;

public:
    FunctionModule(const QString &name, const ModulePackage *parent_pkg) : ParserModule(name, parent_pkg) {}
    FunctionModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const FunctionInterface &interface, const StaticInfo &statics) : ParserModule(name, description, parent_pkg), interface(interface), statics(statics) {}

    bool matches(const std::string &candidate, unsigned int argCount) const;
    std::unique_ptr<CAS::AbstractExpression> parse(const std::string &identifier, std::vector<std::unique_ptr<CAS::AbstractExpression>> &args) const { return interface.parse(identifier, args);}
    unsigned int priority() const;

    friend QDataStream &operator<<(QDataStream &, const FunctionModule &);
    friend QTextStream &operator>>(QTextStream &stream, FunctionModule &module);
};

QDataStream &operator<<(QDataStream &stream, const FunctionModule &module);
QTextStream &operator>>(QTextStream &stream, FunctionModule &module);

#endif //FUNCTIONMODULE_H
