#include "JarvisServer.h"
#include <iostream>

JarvisServer::JarvisServer() : settings("InfoAG", "Jarvis.Server")
{
    //listen(QHostAddress::Any, settings.value("Port").toUInt());
    settings.setValue("Version", 1);
    listen(QHostAddress::Any, 4200);
    parser = std::unique_ptr<ExpressionParser>(new ExpressionParser(QDir(settings.value("ModulePath").toString())));
    /*
    std::string input;
    for (;;) {
        std::cin >> input;
        std::cout << parser->parse(input)->eval(CAS::EvalInfo())->getString() << std::endl;
    }*/
}

const Scope &JarvisServer::enterScope(ClientConnection *client, QString name)
{
    if (! scopes.contains(name)) {
        scopes.insert(name, Scope(name));
        std::for_each(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> it_client) {
                if (it_client.get() != client) it_client->newScope(name);
            });
    }
    scopes[name].addClient(client);
    return scopes[name];
}

void JarvisServer::leaveScope(ClientConnection *sender, QString name)
{
    scopes[name].removeClient(sender);
}

void JarvisServer::msgToScope(ClientConnection *sender, QString scope, QString msg) const
{
    qDebug() << sender->name() << scope << msg;
    scopes[scope].sendMsg(sender->name(), msg);
    scopes[scope].sendMsg("Jarvis", QString::fromStdString(parser->parse(msg.toStdString())->eval(CAS::EvalInfo())->getString()));
}

void JarvisServer::incomingConnection(int socketfd)
{
    auto connection = std::shared_ptr<ClientConnection>(new ClientConnection(this));
    connection->setSocketDescriptor(socketfd);
    clients.append(connection);
}
