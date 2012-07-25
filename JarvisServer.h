#ifndef JARVISSERVER_H
#define JARVISSERVER_H

#include <QSettings>
#include <QTcpServer>
#include <QList>
#include <QMap>
#include <QDir>
#include "ExpressionParser.h"
#include "Scope.h"

class ClientConnection;

class JarvisServer : public QTcpServer
{
    Q_OBJECT

public:
    JarvisServer();
/*
    inline void newScope(QString name) { scopes.append(Scope(name));};
    void sendMsgToScope(QString name) const;
    void getScopeUsers(QString name) const;
*/
    friend class ClientConnection;

protected:
    void incomingConnection(int socketfd);

private:
    QSettings settings;
    ExpressionParser *parser;
    QList<ClientConnection*> clients;
    QMap<QString, Scope> scopes;
};

#endif // JARVISSERVER_H
