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
    ModulePackage() {}
    ModulePackage(std::unique_ptr<QFile> file);

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

QDataStream &operator<<(QDataStream &stream, const ModulePackage &pkg);

#endif // MODULEPACKAGE_H
