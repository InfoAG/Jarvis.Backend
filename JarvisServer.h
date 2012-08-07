#ifndef JARVISSERVER_H
#define JARVISSERVER_H

#include <QSettings>
#include <QTcpServer>
#include <QList>
#include <QMap>
#include <QDir>
#include <memory>
#include "Scope.h"
#include "ExpressionParser.h"

class ClientConnection;

class JarvisServer : public QTcpServer
{
    Q_OBJECT

public:
    JarvisServer();

    bool login(const QString &name, const QString &pwd) const { qDebug() << "ClientLogin(" << name << ", " << pwd << ")"; return true; };
    QList<QString> getScopeNames() const { return scopes.keys(); };
    ExpressionParser *getParser() const { return parser.get(); }

    const Scope &enterScope(ClientConnection *client, QString scope);
    void leaveScope(ClientConnection *sender, QString scope);
    void msgToScope(ClientConnection *sender, QString scope, QString msg) const;
    uint version() const { return settings.value("Version").toUInt(); };
    void disconnected(ClientConnection *client);
    void deleteScope(const QString &name);

protected:
    void incomingConnection(int socketfd);

private:
    QSettings settings;
    std::unique_ptr<ExpressionParser> parser;
    QList<std::shared_ptr<ClientConnection> > clients;
    QMap<QString, Scope> scopes;
};

#endif // JARVISSERVER_H
