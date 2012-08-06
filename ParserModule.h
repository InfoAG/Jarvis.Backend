#ifndef PARSERMODULE_H
#define PARSERMODULE_H

#include <QFile>
//#include <QDataStream>
//#include "ModulePackage.h"

class ModulePackage;

class ParserModule
{
public:
    ParserModule(const QString &name, const QString &description, const ModulePackage *parent_pkg) : name(name), description(description), parent_pkg(parent_pkg) {};

    ParserModule &operator=(const ParserModule &other) { name = other.name; description = other.description; parent_pkg = other.parent_pkg; return *this; }

//private:
    QString name;
    QString description;
    const ModulePackage *parent_pkg;
};

QDataStream &operator<<(QDataStream &stream, const ParserModule &module);

#endif // PARSERMODULE_H
