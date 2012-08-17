#ifndef JARVISSERVER_H
#define JARVISSERVER_H

#include <QSettings>
#include <QCoreApplication>
#include <QTcpServer>
#include <QList>
#include <QMap>
#include <QDir>
#include <memory>
#include "Scope.h"
#include "ExpressionParser.h"

class ClientConnection;

/**
 * Jarvis server which manages client sessions, accounts, settings, scopes and a the parser
 */
class JarvisServer : public QTcpServer
{
    Q_OBJECT

public:
    JarvisServer(); //!< Constructor

    /**
     * Process login request from client
     * @param nick Client nick
     * @param pwd Password
     * @return true if login succeeded
     */
    bool login(const QString &nick, const QString &pwd) const { qDebug() << "ClientLogin(" << nick << ", " << pwd << ")"; return true; };
    QList<QString> getScopeNames() const { return scopes.keys(); }; //!< @return List of scope names
    ExpressionParser *getParser() const { return parser.get(); } //!< @return Pointer to server wide parser

    /**
     * Allow client to enter a scope; if no scope exists with this name, create a new one
     * @param client Pointer to ClientConnection object
     * @param scope Scope name
     * @return Scope object
     */
    const Scope &enterScope(ClientConnection *client, QString scope);
    /**
     * Remove client from scope
     * @param sender Pointer to ClientConnection object
     * @param scope Scope name
     */
    void leaveScope(ClientConnection *sender, QString scope);
    /**
     * Send a message to a scope
     * @param sender Pointer to ClientConnection object
     * @param scope Scope name
     * @param msg Message string
     */
    void msgToScope(ClientConnection *sender, QString scope, QString msg);
    uint version() const { return 1; }; //!< @return Server version as specified in settings
    /**
     * Client disconnected; remove from all scopes
     * @param client Pointer to ClientConnection object
     */
    void disconnected(ClientConnection *client);
    /**
     * Delete a scope
     * @param name Scope name
     */
    void deleteScope(const QString &name);
    /**
     * Unload a parser package
     * @param pkgName Package name
     */
    void unload(const QString &pkgName);
    /**
     * Load a parser package
     * @param pkgName Package name
     */
    void load(const QString &pkgName);

protected:
    /**
     * New client connected, construct ClientConnection object
     * @param socketfd
     */
    void incomingConnection(int socketfd);

private:
    void defaultSetting(const QString &key, const QVariant &value) { if (! settings.contains(key)) settings.setValue(key, value); }

    QSettings settings; //!< Server settings
    std::unique_ptr<ExpressionParser> parser; //!< Server wide dynamic parser
    QList<std::shared_ptr<ClientConnection> > clients; //!< List of all clients
    QMap<QString, Scope> scopes; //!< Maps scope object to names
};

#endif // JARVISSERVER_H
