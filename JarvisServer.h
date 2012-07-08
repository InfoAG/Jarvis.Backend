#ifndef JARVISSERVER_H
#define JARVISSERVER_H

#include <QSettings>
#include <QTcpServer>
#include <QList>
#include <QDir>
#include "ExpressionParser.h"

class ClientConnection;

class JarvisServer : public QTcpServer
{
    Q_OBJECT

public:
    JarvisServer();

    friend class ClientConnection;

protected:
    void incomingConnection(int socketfd);

private:
    QSettings settings;
    ExpressionParser *parser;
    QList<ClientConnection*> clients;
};

#endif // JARVISSERVER_H
