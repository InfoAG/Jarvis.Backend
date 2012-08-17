#ifndef PARSERMODULE_H
#define PARSERMODULE_H

#include <QFile>
//#include <QDataStream>
//#include "ModulePackage.h"

class ModulePackage;

/**
 * Parser
 */
class ParserModule
{
public:
    ParserModule(const QString &name, const QString &description, const ModulePackage *parent_pkg) : name_(name), description_(description), parent_pkg(parent_pkg) {};

    bool inPkg(ModulePackage *pkg) const { return parent_pkg == pkg; }
    QString name() const { return name_; }
    QString description() const { return description_; }

    ParserModule &operator=(const ParserModule &other) { name_ = other.name_; description_ = other.description_; parent_pkg = other.parent_pkg; return *this; }

private:
    QString name_;
    QString description_;
    const ModulePackage *parent_pkg;
};

QDataStream &operator<<(QDataStream &stream, const ParserModule &module);

#endif // PARSERMODULE_H
