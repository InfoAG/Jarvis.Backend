#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QFile>
#include <string>
#include <map>
#include <QLibrary>
#include "ParserModules.h"

class ModulePackage
{
public:
    ModulePackage(QFile *file);

    const ParserModules &getModules() const { return modules; };

//private:
    QString name;
    ParserModules modules;
    enum {
        HEAD,
        BRACE,
        BODY
    } state = HEAD;

    enum {
        OPERATOR,
        FUNCTION,
        TERMINAL
    } type;

};

#endif // MODULEPACKAGE_H
