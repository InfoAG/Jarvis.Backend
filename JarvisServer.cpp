#include "JarvisServer.h"
#include "ClientConnection.h"
#include <iostream>

JarvisServer::JarvisServer() : settings("InfoAG", "Jarvis.Server")
{
    defaultSetting("Port", 4200);
    defaultSetting("ModulePath", QCoreApplication::applicationDirPath() + "/../modules/");
    settings.setValue("ModulePath", QCoreApplication::applicationDirPath() + "/../modules/");
    qDebug() << QCoreApplication::applicationDirPath() + "/modules";
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/modules");
    qDebug() << QCoreApplication::libraryPaths();
    //settings.setValue("Version", 1);
    listen(QHostAddress::Any, settings.value("Port").toUInt());
    parser = std::unique_ptr<ExpressionParser>(new ExpressionParser(QDir(settings.value("ModulePath").toString())));
}

const Scope &JarvisServer::enterScope(ClientConnection *client, QString scope)
{
    if (! scopes.contains(scope)) {
        scopes.insert(scope, Scope(scope, parser.get()));
        for (const auto &it_client: clients) {
            if (it_client.get() != client) it_client->newScope(scope);
        }
    }
    scopes[scope].addClient(client);
    qDebug() << "ClientEnterScope(" << client->nick() << ", " << scope << ")";
    return scopes[scope];
}

void JarvisServer::leaveScope(ClientConnection *sender, QString scope)
{
    scopes[scope].removeClient(sender);
    qDebug() << "ClientLeaveScope(" << sender->nick() << ", " << scope << ")";
}

void JarvisServer::msgToScope(ClientConnection *sender, QString scope, QString msg)
{
    if (scopes.contains(scope) && scopes[scope].hasClient(sender)) {
        scopes[scope].sendMsg(sender->nick(), msg);
        qDebug() << "MsgToScope(" << sender->nick() << ", " << scope << ", " << msg << ")";
    }
}

void JarvisServer::disconnected(ClientConnection *client)
{
    for (auto &scope : scopes) scope.removeClient(client);
    clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> &it_client) { return it_client.get() == client; }));
    qDebug() << "ClientDisconnect(" << client->nick() << ")";
}

void JarvisServer::deleteScope(const QString &name)
{
    for (const auto &client : clients) client->deleteScope(name);
    scopes.remove(name);
    qDebug() << "DeleteScope(" << name << ")";
}

void JarvisServer::unload(const QString &pkgName)
{
    //ugly, compare parser->unload()
    auto modulePkgs = parser->getModulePkgs();
    if (std::find_if(modulePkgs.begin(), modulePkgs.end(), [&](const ModulePackage &it_pkg) {
            return it_pkg.name() == pkgName;
        }) != modulePkgs.end()) {
        parser->unload(pkgName);
        for (const auto &client : clients) client->unloadPkg(pkgName);
        qDebug() << "UnloadPkg(" << pkgName << ")";
    }
}

void JarvisServer::load(const QString &pkgName)
{
    ModulePackage result(*(parser->load(settings.value("ModulePath").toString() + pkgName + ".jpkg")));
    for (const auto &client : clients) client->loadPkg(result);
    qDebug() << "LoadPkg(" << pkgName << ")";
}

void JarvisServer::incomingConnection(int socketfd)
{
    auto client = std::shared_ptr<ClientConnection>(new ClientConnection(this, socketfd), std::mem_fun(&QObject::deleteLater));
    clients.append(client);
    qDebug() << "NewConnection(" << client->getAddress().toString() << ")";
}
