#include "ParserModules.h"

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules)
{
    stream << modules.terminals << modules.binaryOperators << modules.unaryOperators << modules.functions;
    return stream;
}


void ParserModules::removePkg(ModulePackage *pkg)
{
    terminals.erase(std::remove_if(terminals.begin(), terminals.end(), [&](const TerminalModule &module) {
           return module.inPkg(pkg);
       }), terminals.end());
    binaryOperators.erase(std::remove_if(binaryOperators.begin(), binaryOperators.end(), [&](const BinaryOperatorModule &module) {
           return module.inPkg(pkg);
       }), binaryOperators.end());
    unaryOperators.erase(std::remove_if(unaryOperators.begin(), unaryOperators.end(), [&](const UnaryOperatorModule &module) {
           return module.inPkg(pkg);
       }), unaryOperators.end());
    functions.erase(std::remove_if(functions.begin(), functions.end(), [&](const FunctionModule &module) {
           return module.inPkg(pkg);
       }), functions.end());
}
