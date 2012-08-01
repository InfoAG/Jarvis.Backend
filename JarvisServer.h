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
#include "ClientConnection.h"

class JarvisServer : public QTcpServer
{
    Q_OBJECT

public:
    JarvisServer();

    bool login(const QString &name, const QString &pwd) const { qDebug() << name << pwd; return true; };
    QList<QString> getScopeNames() const { return scopes.keys(); };

    const Scope &enterScope(ClientConnection *client, QString name);
    void leaveScope(ClientConnection *sender, QString name);
    void msgToScope(ClientConnection *sender, QString scope, QString msg) const;
    uint version() const { return settings.value("Version").toUInt(); };

protected:
    void incomingConnection(int socketfd);

private:
    QSettings settings;
    std::unique_ptr<ExpressionParser> parser;
    QList<std::shared_ptr<ClientConnection> > clients;
    QMap<QString, Scope> scopes;
};

#endif // JARVISSERVER_H
