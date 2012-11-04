#include "RoomScope.h"

void RoomScope::declareVar(CAS::TypeInfo type, std::string id)
{
    CAS::Scope::declareVar(type, id);
    emit declaredVar(type, id);
}

void RoomScope::declareFunc(CAS::FunctionSignature sig, CAS::TypeInfo returnType)
{
    CAS::Scope::declareFunc(sig, returnType);
    emit declaredFunc(sig, returnType);
}

void RoomScope::defineVar(const std::string &id, CAS::VariableDefinition var)
{
    CAS::Scope::defineVar(id, var);
    emit definedVar(id, var);
}

void RoomScope::defineFunc(const CAS::FunctionSignature &sig, CAS::FunctionDefinition def)
{
    CAS::Scope::defineFunc(sig, def);
    emit definedFunc(sig, def);
}

QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope)
{
    stream << static_cast<quint32>(roomScope.variables.size());
    for (const auto &item : roomScope.variables)
        stream << QString::fromStdString(item.first) << QString::fromStdString(item.second.type.toString()) << (item.second.definition == nullptr ? QString() : QString::fromStdString(item.second.definition->toString()));
    stream << static_cast<quint32>(roomScope.functions.size());
    for (const auto &item : roomScope.functions) {
        stream << QString::fromStdString(item.first.id) << static_cast<quint32>(item.first.argumentTypes.size());
        for (const auto &argType : item.first.argumentTypes)
            stream << QString::fromStdString(argType.toString());
        stream << QString::fromStdString(item.second.returnType.toString()) << static_cast<quint32>(item.second.arguments.size());
        for (const auto &argStr : item.second.arguments)
            stream << QString::fromStdString(argStr);
        stream << (item.second.definition == nullptr ? QString() : QString::fromStdString(item.second.definition->toString()));
    }
    return stream;
}
