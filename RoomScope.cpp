#include "RoomScope.h"

void RoomScope::assignVar(const std::pair<std::string, CAS::Definition> &var)
{
    emit newVariable(var);
    CAS::Scope::assignVar(var);
}

void RoomScope::assignFunc(const std::pair<std::string, CAS::FunctionDefinition> &func)
{
    emit newFunction(func);
    CAS::Scope::assignFunc(func);
}
