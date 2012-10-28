#include "ParserModules.h"

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules)
{
    stream << static_cast<quint32>(modules.terminals.size());
    for (const auto &mod : modules.terminals) stream << *mod;
    stream << static_cast<quint32>(modules.binaryOperators.size());
    for (const auto &mod : modules.binaryOperators) stream << *mod;
    stream << static_cast<quint32>(modules.unaryOperators.size());
    for (const auto &mod : modules.unaryOperators) stream << *mod;
    stream << static_cast<quint32>(modules.functions.size());
    for (const auto &mod : modules.functions) stream << *mod;
    return stream;
}


void ParserModules::removePkg(ModulePackage *pkg)
{
    terminals.erase(std::remove_if(terminals.begin(), terminals.end(), [&](const std::shared_ptr<TerminalModule> &module) {
           return module->inPkg(pkg);
       }), terminals.end());
    binaryOperators.erase(std::remove_if(binaryOperators.begin(), binaryOperators.end(), [&](const std::shared_ptr<BinaryOperatorModule> &module) {
           return module->inPkg(pkg);
       }), binaryOperators.end());
    unaryOperators.erase(std::remove_if(unaryOperators.begin(), unaryOperators.end(), [&](const std::shared_ptr<UnaryOperatorModule> &module) {
           return module->inPkg(pkg);
       }), unaryOperators.end());
    functions.erase(std::remove_if(functions.begin(), functions.end(), [&](const std::shared_ptr<FunctionModule> &module) {
           return module->inPkg(pkg);
       }), functions.end());
}
