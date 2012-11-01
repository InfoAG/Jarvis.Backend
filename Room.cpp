#include "Room.h"
#include "ClientConnection.h"

Room::Room(const QString &name, ExpressionParser *parser) : name(name), parser(parser), roomScope(new RoomScope)
{
    connect(roomScope.get(), SIGNAL(declaredVar(CAS::AbstractExpression::ReturnType, const std::string &)), SLOT(declaredVar(CAS::AbstractExpression::ReturnType, const std::string &)));
    connect(roomScope.get(), SIGNAL(declaredFunc(const CAS::FunctionSignature &, CAS::AbstractExpression::ReturnType)), SLOT(declaredFunc(const CAS::FunctionSignature &, CAS::AbstractExpression::ReturnType)));
    connect(roomScope.get(), SIGNAL(definedFunc(const CAS::FunctionSignature &, const CAS::FunctionDefinition &)), SLOT(definedFunc(const CAS::FunctionSignature &, const CAS::FunctionDefinition &)));
    connect(roomScope.get(), SIGNAL(definedVar(const std::string &, const CAS::VariableDefinition &)), SLOT(definedVar(const std::string &, const CAS::VariableDefinition &)));
}

void Room::getInitInfo(QDataStream &stream) const
{   
    stream << static_cast<quint32>(clients.length());
    for (const auto &client : clients) stream << client->nick();
    stream << *roomScope;
    /*stream << static_cast<quint32>(roomScope->getVars().size());
    for (const auto &item : roomScope->getVars())
            stream << QString::fromStdString(item.first) << item.second;
    stream << static_cast<quint32>(roomScope->getFunc().size());
    for (const auto &item : roomScope->getFunc())
        stream << QString::fromStdString(item.first) << item.second;*/
}

void Room::removeClient(ClientConnection *client)
{
    if (clients.contains(client)) {
        clients.removeOne(client);
       for (const auto &it_client : clients) it_client->leaveClient(name, client->nick());
    }
}

void Room::sendMsg(const QString &sender, const QString &msg)
{
    for (const auto &client : clients) client->sendMsg(name, sender, msg);
    try {
        auto result = parser->parse(msg.toStdString())->eval(*roomScope).second;
        auto resultString = QString::fromStdString(result->toString());
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", resultString);
    } catch (const char *s) {
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", s);
    }
}

void Room::addClient(ClientConnection *client)
{
    if (clients.contains(client)) throw 0;
    for (const auto &it_client : clients) it_client->enterClient(name, client->nick());
    clients.append(client);
}

void Room::declaredVar(CAS::AbstractExpression::ReturnType type, const std::string &id)
{
    QString qID = QString::fromStdString(id), qType = QString::fromStdString(CAS::AbstractExpression::typeToString(type));
    qDebug() << "DeclaredVar(" << qType << ", " << qID << ")";
    for (const auto &client : clients)
        client->declaredVar(name, qType, qID);
}

void Room::definedVar(const std::string &id, const CAS::VariableDefinition &var)
{
    QString defStr = QString::fromStdString(var.definition->toString());
    QString qID = QString::fromStdString(id);
    qDebug() << "DefinedVar(" << qID << ", " << defStr << ")";
    for (const auto &client : clients)
        client->definedVar(name, qID, defStr);
}

void Room::declaredFunc(const CAS::FunctionSignature &sig, CAS::AbstractExpression::ReturnType returnType)
{
    QString qID = QString::fromStdString(sig.id), qReturnType = QString::fromStdString(CAS::AbstractExpression::typeToString(returnType));
    QStringList qArgTypes;
    for (const auto &arg : sig.argumentTypes) qArgTypes.append(QString::fromStdString(CAS::AbstractExpression::typeToString(arg)));
    qDebug() << "FunctionDeclaration(" << qID << ", (" << qArgTypes.join(", ") << "), " << qReturnType;
    for (const auto &client : clients) client->declaredFunc(name, qID, qArgTypes, qReturnType);
}

void Room::definedFunc(const CAS::FunctionSignature &sig, const CAS::FunctionDefinition &def)
{
    QString qID = QString::fromStdString(sig.id);
    qDebug() << "NewFunction(" << qID << ", (";
    auto itArgStrs = def.arguments.cbegin();
    QList<QPair<QString, QString>> arguments;
    for (const auto &argType : sig.argumentTypes) {
        auto qArgStr = QString::fromStdString(*(itArgStrs++)), qArgType = QString::fromStdString(CAS::AbstractExpression::typeToString(argType));
        arguments.append(qMakePair(qArgType, qArgStr));
        qDebug() << qArgType << " " << qArgStr;
        if (itArgStrs != def.arguments.cend()) qDebug() << ", ";
    }
    QString defStr = QString::fromStdString(def.definition->toString());
    qDebug() << "), " << defStr << ")";
    for (const auto &client : clients)
        client->definedFunc(name, qID, arguments, defStr);
}

