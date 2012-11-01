#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QTimer>
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
        EnterRoom,
        LeaveRoom,
        ClientMsg,
        UnloadPkg,
        LoadPkg,
        DeleteRoom
    } connectionState = Virgin; //!< Connection state

    QTcpSocket socket; //!< Socket with client connection
    JarvisServer *server; //!< Parent server
    QDataStream iStream, oStream; //!< Input and output streams for socket
    QString _nick; //!< Client nick
    QByteArray streamBuf; //!< Stream buffer, needed because QDataStream can fail after stealing all the data from QTcpSocket    
    char pingCount{0};
    QTimer inactivityTimer;

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
    ~ClientConnection() { socket.disconnect(); }

    QString nick() const { return _nick; }; //!< @return Client nick
    /**
     * Signalize a new message
     * @param room Room name
     * @param sender Sender name
     * @param msg Message string
     */
    void sendMsg(const QString &room, const QString &sender, const QString &msg) { oStream << static_cast<quint8>(4) << room << sender << msg; };
    /**
     * Signalize creation of a new room
     * @param name Room name
     */
    void newRoom(const QString &name) { oStream << static_cast<quint8>(3) << name; };
    /**
     * A client entered one of the observed rooms
     * @param room Room name
     * @param name Client name
     */
    void enterClient(const QString &room, const QString &name) { oStream << static_cast<quint8>(0) << room << name; }
    /**
     * A client left one of the observed rooms
     * @param room Room name
     * @param name Client name
     */
    void leaveClient(const QString &room, const QString &name) { oStream << static_cast<quint8>(5) << room << name; }
    /**
     * A room was deleted
     * @param room Room name
     */
    void deleteRoom(const QString &room) { oStream << static_cast<quint8>(9) << room; }
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
    void declaredVar(const QString &room, const QString &type, const QString &id) { oStream << static_cast<quint8>(2) << room << type << id; }
    void definedVar(const QString &room, const QString &id, const QString &definition) { oStream << static_cast<quint8>(11) << room << id << definition; }
    void declaredFunc(const QString &room, const QString &identifier, const QStringList &argumentTypes, const QString &returnType);
    void definedFunc(const QString &room, const QString &identifier, const QList<QPair<QString, QString>> &arguments, const QString &definition);
    QHostAddress getAddress() const { return socket.peerAddress(); } //!< @return Client address

private slots:
    void readyRead(); //!< Data available to read from socket
    void disconnected() { server->disconnected(this); } //!< Socket disconnected
    void timeout();
};

#endif // CLIENTCONNECTION_H
