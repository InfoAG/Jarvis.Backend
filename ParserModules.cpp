#include "ParserModules.h"

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules)
{
    stream << modules.terminals << modules.operators << modules.functions;
    return stream;
}


void ParserModules::removePkg(ModulePackage *pkg)
{
    terminals.erase(std::remove_if(terminals.begin(), terminals.end(), [&](const TerminalModule &module) {
           return module.inPkg(pkg);
       }), terminals.end());
    operators.erase(std::remove_if(operators.begin(), operators.end(), [&](const OperatorModule &module) {
           return module.inPkg(pkg);
       }), operators.end());
    functions.erase(std::remove_if(functions.begin(), functions.end(), [&](const FunctionModule &module) {
           return module.inPkg(pkg);
       }), functions.end());
}
