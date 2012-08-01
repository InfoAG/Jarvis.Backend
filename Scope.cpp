#include "Scope.h"

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
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *client) {
            client->leaveClient(name, client->name());
        });
}

void Scope::sendMsg(const QString &sender, const QString &msg) const
{
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *client) {
            client->sendMsg(name, sender, msg);
        });
}

void Scope::addClient(ClientConnection *client)
{
    std::for_each(clients.begin(), clients.end(), [&](ClientConnection *client) {
            client->enterClient(name, client->name());
        });
    clients.append(client);
}
