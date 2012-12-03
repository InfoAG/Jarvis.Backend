#ifndef JARVISSERVER_H
#define JARVISSERVER_H

#include <QSettings>
#include <QCoreApplication>
#include <QTcpServer>
#include <QList>
#include <QMap>
#include <QDir>
#include <memory>
#include "Room.h"
#include "ExpressionParser.h"

class ClientConnection;

/**
 * Jarvis server which manages client sessions, accounts, settings, rooms and a the parser
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
    QList<QString> getRoomNames() const { return rooms.keys(); } //!< @return List of room names
    ExpressionParser *getParser() const { return parser.get(); } //!< @return Pointer to server wide parser

    /**
     * Allow client to enter a room; if no room exists with this name, create a new one
     * @param client Pointer to ClientConnection object
     * @param room Room name
     * @return Room object
     */
    const Room *enterRoom(ClientConnection *client, QString room);
    /**
     * Remove client from room
     * @param sender Pointer to ClientConnection object
     * @param room Room name
     */
    void leaveRoom(ClientConnection *sender, QString room);
    /**
     * Send a message to a room
     * @param sender Pointer to ClientConnection object
     * @param room Room name
     * @param msg Message string
     */
    void msgToRoom(ClientConnection *sender, QString room, QString msg);
    uint version() const { return 1; }; //!< @return Server version as specified in settings
    /**
     * Client disconnected; remove from all rooms
     * @param client Pointer to ClientConnection object
     */
    void disconnected(ClientConnection *client);
    /**
     * Delete a room
     * @param name Room name
     */
    void deleteRoom(const QString &name);
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
    std::pair<std::unique_ptr<QFile>, QString> resolveImport(const QString &fileName) const { return std::make_pair(make_unique<QFile>("../cextensions/" + fileName + ".jce"), "cextensions/" + fileName); }

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
    QMap<QString, std::shared_ptr<Room>> rooms; //!< Maps room object to names
};

#endif // JARVISSERVER_H
