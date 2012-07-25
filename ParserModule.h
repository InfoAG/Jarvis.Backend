#ifndef PARSERMODULE_H
#define PARSERMODULE_H

#include <QFile>

class ModulePackage;

class ParserModule
{
public:
    ParserModule(const QString &name, const QString &description, const ModulePackage &parent_pkg) : name(name), description(description), parent_pkg(parent_pkg) {};

//private:
    QString name;
    QString description;
    const ModulePackage &parent_pkg;
};

#endif // PARSERMODULE_H
