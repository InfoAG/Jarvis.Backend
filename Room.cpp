#include "Room.h"
#include "ClientConnection.h"
#include "RoomScope.h"

Room::Room(const QString &name, ExpressionParser *parser, const JarvisServer &server) : name(name), parser(parser), roomScope(std::make_shared<RoomScope>(server))
{
    connect(roomScope.get(), SIGNAL(declaredVar(const CAS::TypeInfo &, const std::string &)), SLOT(declaredVar(const CAS::TypeInfo &, const std::string &)));
    connect(roomScope.get(), SIGNAL(declaredFunc(const CAS::FunctionSignature &, const CAS::TypeInfo &)), SLOT(declaredFunc(const CAS::FunctionSignature &, const CAS::TypeInfo &)));
    connect(roomScope.get(), SIGNAL(definedFunc(const CAS::FunctionSignature &, const CAS::FunctionDefinition &)), SLOT(definedFunc(const CAS::FunctionSignature &, const CAS::FunctionDefinition &)));
    connect(roomScope.get(), SIGNAL(definedVar(const std::string &, const CAS::AbstractExpression::ExpressionP &)), SLOT(definedVar(const std::string &, const CAS::AbstractExpression::ExpressionP &)));
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
        auto result = parser->parse(msg.toStdString());
        result->typeCheck(CAS::TypeCollection::all(), *roomScope);
        result = result->eval(*roomScope, std::bind(&RoomScope::load, roomScope.get(), std::placeholders::_1));
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

void Room::declaredVar(const CAS::TypeInfo &type, const std::string &id)
{
    QString qID = QString::fromStdString(id), qType = QString::fromStdString(type.toString());
    qDebug() << "DeclaredVar(" << qType << ", " << qID << ")";
    for (const auto &client : clients)
        client->declaredVar(name, qType, qID);
}

void Room::definedVar(const std::string &id, const CAS::AbstractExpression::ExpressionP &definition)
{
    QString defStr = QString::fromStdString(definition->toString());
    QString qID = QString::fromStdString(id);
    qDebug() << "DefinedVar(" << qID << ", " << defStr << ")";
    for (const auto &client : clients)
        client->definedVar(name, qID, defStr);
}

void Room::declaredFunc(const CAS::FunctionSignature &sig, const CAS::TypeInfo &returnType)
{
    QString qID = QString::fromStdString(sig.id), qReturnType = QString::fromStdString(returnType.toString());
    QStringList qArgTypes;
    for (const auto &arg : sig.argumentTypes) qArgTypes.append(QString::fromStdString(arg.toString()));
    qDebug() << "FunctionDeclaration(" << qID << ", (" << qArgTypes.join(", ") << "), " << qReturnType;
    for (const auto &client : clients) client->declaredFunc(name, qID, qArgTypes, qReturnType);
}

void Room::definedFunc(const CAS::FunctionSignature &sig, const CAS::FunctionDefinition &def)
{
    QString qID = QString::fromStdString(sig.id);
    qDebug() << "FunctionDefinition(" << qID << ", (";
    auto itArgStrs = def.arguments.cbegin();
    QList<QPair<QString, QString>> arguments;
    for (const auto &argType : sig.argumentTypes) {
        auto qArgStr = QString::fromStdString(*(itArgStrs++)), qArgType = QString::fromStdString(argType.toString());
        arguments.append(qMakePair(qArgType, qArgStr));
        qDebug() << qArgType << " " << qArgStr;
        if (itArgStrs != def.arguments.cend()) qDebug() << ", ";
    }
    QString defStr = QString::fromStdString(def.definition->toString());
    qDebug() << "), " << defStr << ")";
    for (const auto &client : clients)
        client->definedFunc(name, qID, arguments, defStr);
}

