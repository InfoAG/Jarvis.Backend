#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include "JarvisServer.h"

/**
 * Speaks to a Jarvis client over network and maintains connection state
 */
class ClientConnection : public QObject
{
    Q_OBJECT

private:
    enum {
        Virgin,
        Auth,
        Loop,
        EnterScope,
        LeaveScope,
        ClientMsg,
        UnloadPkg,
        LoadPkg,
        DeleteScope
    } connectionState = Virgin; //!< Connection state

    QTcpSocket socket; //!< Socket with client connection
    JarvisServer *server; //!< Parent server
    QDataStream iStream, oStream; //!< Input and output streams for socket
    QString _nick; //!< Client nick
    QByteArray streamBuf; //!< Stream buffer, needed because QDataStream can fail after stealing all the data from QTcpSocket

    /**
     * Pop first byte from streamBuf
     * @return the byte bro. just the byte.
     */
    quint8 pop_front() { quint8 result(streamBuf.at(0)); streamBuf.remove(0, 1); return result; }
    void resetStreamBuf() { streamBuf.remove(0, iStream.device()->pos()); } //!< Remove all bytes from streamBuf which have been read through iStream

public:
    /**
     * Constructor
     * @param server Parent server
     * @param socketfd Socket descriptor
     */
    ClientConnection(JarvisServer *server, int socketfd);

    QString nick() const { return _nick; }; //!< @return Client nick
    /**
     * Signalize a new message
     * @param scope Scope name
     * @param sender Sender name
     * @param msg Message string
     */
    void sendMsg(const QString &scope, const QString &sender, const QString &msg) { oStream << static_cast<quint8>(4) << scope << sender << msg; };
    /**
     * Signalize creation of a new scope
     * @param name Scope name
     */
    void newScope(const QString &name) { oStream << static_cast<quint8>(3) << name; };
    /**
     * A client entered one of the observed scopes
     * @param scope Scope name
     * @param name Client name
     */
    void enterClient(const QString &scope, const QString &name) { oStream << static_cast<quint8>(0) << scope << name; }
    /**
     * A client left one of the observed scopes
     * @param scope Scope name
     * @param name Client name
     */
    void leaveClient(const QString &scope, const QString &name) { oStream << static_cast<quint8>(5) << scope << name; }
    /**
     * A scope was deleted
     * @param scope Scope name
     */
    void deleteScope(const QString &scope) { oStream << static_cast<quint8>(9) << scope; }
    /**
     * Module package unloaded
     * @param name Package name
     */
    void unloadPkg(const QString &name) { oStream << static_cast<quint8>(7) << name; }
    /**
     * New module package loaded
     * @param pkg Package content
     */
    void loadPkg(const ModulePackage &pkg) { oStream << static_cast<quint8>(6) << pkg; }
    void newVariable(const QString &scope, const QString &identifier, const QString &definition) { oStream << static_cast<quint8>(2) << scope << identifier << definition; }
    void newFunction(const QString &scope, const QString &identifier, const QStringList &arguments, const QString &definition) { oStream << static_cast<quint8>(1) << scope << identifier << arguments << definition; }
    QHostAddress getAddress() const { return socket.peerAddress(); } //!< @return Client address

private slots:
    void readyRead(); //!< Data available to read from socket
    void disconnected() { server->disconnected(this); } //!< Socket disconnected
};

#endif // CLIENTCONNECTION_H
