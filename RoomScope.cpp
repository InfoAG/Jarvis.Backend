#include "RoomScope.h"

void RoomScope::declareVar(CAS::AbstractExpression::ReturnType type, std::string id)
{
    emit declaredVar(type, id);
    CAS::Scope::declareVar(type, std::move(id));
}

void RoomScope::declareFunc(CAS::FunctionSignature sig, CAS::FunctionDefinition def)
{
    emit newFunction(sig, def);
    CAS::Scope::declareFunc(std::move(sig), std::move(def));
}

void RoomScope::defineVar(const std::string &id, CAS::VariableDefinition var)
{
    emit definedVar(id, var);
    CAS::Scope::defineVar(id, std::move(var));
}

QString RoomScope::expressionTypeToString(CAS::AbstractExpression::ReturnType type)
{
    switch (type) {
    case CAS::AbstractExpression::NUMBER: return "NUMBER";
    case CAS::AbstractExpression::BOOL: return "BOOL";
    case CAS::AbstractExpression::LIST: return "LIST";
    }
    return "fsda";
}

QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope)
{
    stream << static_cast<quint32>(roomScope.variables.size());
    for (const auto &item : roomScope.variables)
        stream << QString::fromStdString(item.first) << RoomScope::expressionTypeToString(item.second.type) << QString::fromStdString(item.second.definition->toString());
    stream << static_cast<quint32>(roomScope.functions.size());
    for (const auto &item : roomScope.functions) {
        stream << QString::fromStdString(item.first.id) << static_cast<quint32>(item.first.argumentTypes.size());
        for (const auto &argType : item.first.argumentTypes)
            stream << RoomScope::expressionTypeToString(argType);
        stream << static_cast<quint32>(item.second.arguments.size());
        for (const auto &argStr : item.second.arguments)
            stream << QString::fromStdString(argStr);
        stream << QString::fromStdString(item.second.definition->toString());
    }
    return stream;
}
