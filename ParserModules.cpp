#include "ParserModules.h"

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules)
{
    stream << static_cast<quint32>(modules.terminalExpressions.size() + modules.terminalStatements.size());
    for (const auto &mod : modules.terminalExpressions) stream << *mod;
    for (const auto &mod : modules.terminalStatements) stream << *mod;
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
    terminalStatements.erase(std::remove_if(terminalStatements.begin(), terminalStatements.end(), [&](const std::shared_ptr<TerminalStatementModule> &module) {
           return module->inPkg(pkg);
       }), terminalStatements.end());
    terminalExpressions.erase(std::remove_if(terminalExpressions.begin(), terminalExpressions.end(), [&](const std::shared_ptr<TerminalExpressionModule> &module) {
           return module->inPkg(pkg);
       }), terminalExpressions.end());
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
