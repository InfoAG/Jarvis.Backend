#include "Scope.h"
#include "ClientConnection.h"

void Scope::getInitInfo(QDataStream &stream) const
{   
    stream << static_cast<quint32>(clients.length());
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *client) {
            stream << client->name();
        });
//stream << scope_info;
}

void Scope::removeClient(ClientConnection *client)
{
    clients.removeOne(client);
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *it_client) {
            it_client->leaveClient(name, client->name());
        });
}

void Scope::sendMsg(const QString &sender, const QString &msg) const
{
    QString evalRes = QString::fromStdString(parser->parse(msg.toStdString())->eval(scope_info)->getString());
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *client) {
            client->sendMsg(name, sender, msg);
            client->sendMsg(name, "Jarvis", evalRes);
        });
}

void Scope::addClient(ClientConnection *client)
{
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *it_client) {
            it_client->enterClient(name, client->name());
        });
    clients.append(client);
}
