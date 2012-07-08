#include "JarvisServer.h"
#include "ClientConnection.h"

JarvisServer::JarvisServer() : settings("InfoAG", "Jarvis.Server")
{
    listen(QHostAddress::Any, settings.value("Port").toUInt());
    parser = new ExpressionParser(QDir(settings.value("ModulePath").toString()));
}

void JarvisServer::incomingConnection(int socketfd)
{
    ClientConnection *connection = new ClientConnection;
    connection->setSocketDescriptor(socketfd);
    clients.append(connection);
}
