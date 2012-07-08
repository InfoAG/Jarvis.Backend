#ifndef PARSERMODULE_H
#define PARSERMODULE_H

#include <QFile>

class ModulePackage;

class ParserModule
{
public:
    ParserModule(const ModulePackage &parent_pkg) : parent_pkg(parent_pkg) {};

private:
    const ModulePackage &parent_pkg;
};

#endif // PARSERMODULE_H
