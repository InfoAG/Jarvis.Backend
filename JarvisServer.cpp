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
    listen(QHostAddress::AnyIPv4, settings.value("Port").toUInt());
    parser = make_unique<ExpressionParser>(QDir(settings.value("ModulePath").toString()));
}

const Room *JarvisServer::enterRoom(ClientConnection *client, QString room)
{
    if (! rooms.contains(room)) {
        rooms.insert(room, make_unique<Room>(room, parser.get(), *this));
        for (const auto &it_client: clients) {
            if (it_client.get() != client) it_client->newRoom(room);
        }
    }
    rooms[room]->addClient(client);
    qDebug() << "ClientEnterRoom(" << client->nick() << ", " << room << ")";
    return rooms[room].get();
}

void JarvisServer::leaveRoom(ClientConnection *sender, QString room)
{
    rooms[room]->removeClient(sender);
    qDebug() << "ClientLeaveRoom(" << sender->nick() << ", " << room << ")";
}

void JarvisServer::msgToRoom(ClientConnection *sender, QString room, QString msg)
{
    if (rooms.contains(room) && rooms[room]->hasClient(sender)) {
        rooms[room]->sendMsg(sender->nick(), msg);
        qDebug() << "MsgToRoom(" << sender->nick() << ", " << room << ", " << msg << ")";
    }
}

void JarvisServer::disconnected(ClientConnection *client)
{
    qDebug() << "ClientDisconnect(" << client->nick() << ")";
    for (auto &room : rooms) room->removeClient(client);
    clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> &it_client) { return it_client.get() == client; }));
}

void JarvisServer::deleteRoom(const QString &name)
{
    for (const auto &client : clients) client->deleteRoom(name);
    rooms.remove(name);
    qDebug() << "DeleteRoom(" << name << ")";
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
