#include "JarvisServer.h"
#include "ClientConnection.h"
#include <iostream>

JarvisServer::JarvisServer() : settings("InfoAG", "Jarvis.Server")
{
    //listen(QHostAddress::Any, settings.value("Port").toUInt());
    settings.setValue("Version", 1);
    listen(QHostAddress::Any, 4200);
    parser = std::unique_ptr<ExpressionParser>(new ExpressionParser(QDir(settings.value("ModulePath").toString())));
}

const Scope &JarvisServer::enterScope(ClientConnection *client, QString scope)
{
    if (! scopes.contains(scope)) {
        scopes.insert(scope, Scope(scope, parser.get()));
        std::for_each(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> &it_client) {
                if (it_client.get() != client) it_client->newScope(scope);
            });
    }
    scopes[scope].addClient(client);
    qDebug() << "ClientEnterScope(" << client->name() << ", " << scope << ")";
    return scopes[scope];
}

void JarvisServer::leaveScope(ClientConnection *sender, QString scope)
{
    scopes[scope].removeClient(sender);
    qDebug() << "ClientLeaveScope(" << sender->name() << ", " << scope << ")";
}

void JarvisServer::msgToScope(ClientConnection *sender, QString scope, QString msg) const
{
    scopes[scope].sendMsg(sender->name(), msg);
    qDebug() << "MsgToScope(" << sender->name() << ", " << scope << ", " << msg << ")";
}

void JarvisServer::disconnected(ClientConnection *client)
{
    QList<Scope> scopeValues = scopes.values();
    std::for_each(scopeValues.begin(), scopeValues.end(), [&](Scope &scope) { scope.removeClient(client); });
    clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> &it_client) { return it_client.get() == client; }));
    qDebug() << "ClientDisconnect(" << client->name() << ")";
}

void JarvisServer::deleteScope(const QString &name)
{
    std::for_each(clients.begin(), clients.end(), [&](std::shared_ptr<ClientConnection> client) {
            client->deleteScope(name);
        });
    scopes.remove(name);
    qDebug() << "DeleteScope(" << name << ")";
}

void JarvisServer::unload(const QString &pkgName)
{
    parser->unload(pkgName);
    std::for_each(clients.begin(), clients.end(), [&](std::shared_ptr<ClientConnection> client) {
            client->unloadPkg(pkgName);
        });
    qDebug() << "UnloadPkg(" << pkgName << ")";
}

void JarvisServer::load(const QString &pkgName)
{
    ModulePackage result(*(parser->load(pkgName)));
    std::for_each(clients.begin(), clients.end(), [&](std::shared_ptr<ClientConnection> client) {
            client->loadPkg(result);
        });
    qDebug() << "LoadPkg(" << pkgName << ")";
}

void JarvisServer::incomingConnection(int socketfd)
{
    auto client = std::shared_ptr<ClientConnection>(new ClientConnection(this, socketfd), std::mem_fun(&QObject::deleteLater));
    clients.append(client);
    qDebug() << "NewConnection(" << client->getAddress().toString() << ")";
}
