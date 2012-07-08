#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QFile>
#include <string>
#include "ParserModules.h"

class ModulePackage
{
public:
    ModulePackage(QFile *file);

    const ParserModules &getModules() const { return modules; };

private:
    std::string name;
    ParserModules modules;
};

#endif // MODULEPACKAGE_H
