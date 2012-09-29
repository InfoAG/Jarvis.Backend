#include "Room.h"
#include "ClientConnection.h"

Room::Room(const QString &name, ExpressionParser *parser) : name(name), parser(parser), roomScope(new RoomScope)
{
    connect(roomScope.get(), SIGNAL(newVariable(const std::pair<std::string, CAS::Definition> &)), SLOT(newVariable(const std::pair<std::string, CAS::Definition> &)));
    connect(roomScope.get(), SIGNAL(newFunction(const std::pair<std::string, CAS::FunctionDefinition> &)), SLOT(newFunction(const std::pair<std::string, CAS::FunctionDefinition> &)));
}

void Room::getInitInfo(QDataStream &stream) const
{   
    stream << static_cast<quint32>(clients.length());
    for (const auto &client : clients) stream << client->nick();
    stream << static_cast<quint32>(roomScope->getVars().size());
    for (const auto &item : roomScope->getVars())
            stream << QString::fromStdString(item.first) << QString::fromStdString(item.second.definition->toString());
    stream << static_cast<quint32>(roomScope->getFunc().size());
    for (const auto &item : roomScope->getFunc()) {
        stream << QString::fromStdString(item.first.first) << static_cast<quint32>(item.second.arguments.size());
        for (const auto &arg : item.second.arguments) stream << QString::fromStdString(arg);
        stream << QString::fromStdString(item.second.definition->toString());
    }
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
        auto result = parser->parse(msg.toStdString())->eval(*(roomScope.get()));
        auto resultString = QString::fromStdString(result->toString());
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", resultString);
    } catch (const char *) {
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", "error bro");
    }
}

void Room::addClient(ClientConnection *client)
{
    if (clients.contains(client)) throw 0;
    for (const auto &it_client : clients) it_client->enterClient(name, client->nick());
    clients.append(client);
}

void Room::newVariable(const std::pair<std::string, CAS::Definition> &var)
{
    QString assignedString = QString::fromStdString(var.second.definition->toString());
    for (const auto &client : clients)
        client->newVariable(name, QString::fromStdString(var.first), assignedString);
    qDebug() << "NewVariable(" << name << ", " << QString::fromStdString(var.first) << ", " << assignedString << ")";
}

void Room::newFunction(const std::pair<std::string, CAS::FunctionDefinition> &func)
{
    QStringList argStrings;
    for (const auto &arg : func.second.arguments) argStrings.append(QString::fromStdString(arg));
    QString assignedString = QString::fromStdString(func.second.definition->toString());
    for (const auto &client : clients)
            client->newFunction(name, QString::fromStdString(func.first), argStrings, assignedString);
    qDebug() << "NewFunction(" << name << ", " << QString::fromStdString(func.first) << ", " << argStrings << ", " <<  assignedString << ")";
}
