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

void RoomScope::defineVar(const std::string &id, CAS::AbstractExpression::ExpressionP definition, bool recursion)
{
    CAS::Scope::defineVar(id, definition->copy(), recursion);
    emit definedVar(id, definition);
}

void RoomScope::defineFunc(const CAS::FunctionSignature &sig, std::vector<std::string> args, CAS::AbstractExpression::ExpressionP def)
{
    CAS::Scope::defineFunc(sig, args, def->copy());
    emit definedFunc(sig, args, def);
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


void RoomScope::load(const std::string &fileName)
{
    auto files = server.resolveImport(QString::fromStdString(fileName));
    files.first->open(QFile::ReadOnly);
    QTextStream fileStream(files.first.get());
    std::vector<CAS::TypeInfo> argTypes;
    while (! fileStream.atEnd()) {
        QString name, returnType, buf;
        fileStream >> name >> buf;
        if (buf != "{") throw "failer";
        fileStream >> returnType;
        if (returnType.endsWith(',')) returnType.chop(1);
        for (;;) {
            fileStream >> buf;
            if (buf == "}") break;
            else if (buf.endsWith(',')) buf.chop(1);
            argTypes.emplace_back(CAS::TypeInfo::fromString(buf.toStdString()));
        }
        CAS::FunctionSignature sig{name.toStdString(), std::move(argTypes)};
        auto type = CAS::TypeInfo::fromString(returnType.toStdString());
        libScope->declareFunc(sig, std::move(type));
        libScope->defineFunc(sig, std::vector<std::string>(), make_unique<CAS::CFunctionBody>(CAS::TypeInfo::fromString(returnType.toStdString()), name.toStdString(), std::function<CAS::AbstractExpression::ExpressionP(const CAS::AbstractExpression::Operands &, Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)>{(CAS::AbstractExpression::ExpressionP(*)(const CAS::AbstractExpression::Operands &, Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption))QLibrary::resolve(files.second, (name + "_jce").toLatin1().data())}));
        argTypes.clear();
        fileStream.skipWhiteSpace();
    }
}
