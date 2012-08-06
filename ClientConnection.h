#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include "JarvisServer.h"

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
        LoadPkg
    } connectionState = Virgin;

    QTcpSocket socket;
    JarvisServer *server;
    QDataStream iStream, oStream, sendQueueStream;
    QString _name;
    QString buffer;
    QByteArray streamBuf;
    quint32 nextBlockSize;
    quint8 type;
    QByteArray sendQueue;

    void setLoop() { connectionState = Loop; dispatchQueue(); }
    void addedToQueue() { if (connectionState == Loop) dispatchQueue(); }
    void dispatchQueue() { socket.write(sendQueue); sendQueue.clear(); sendQueueStream.device()->reset(); }
    quint8 pop_front() { quint8 result(streamBuf.at(0)); streamBuf.remove(0, 1); return result; }
    void resetStreamBuf() { streamBuf.remove(0, iStream.device()->pos()); }

public:
    ClientConnection(JarvisServer *server, int socketfd);

    QString name() const { return _name; };
    void sendMsg(const QString &scope, const QString &sender, const QString &msg) { sendQueueStream << static_cast<quint8>(4) << scope << sender << msg; addedToQueue(); };
    void newScope(const QString &name) { sendQueueStream << static_cast<quint8>(3) << name; addedToQueue(); };
    void enterClient(const QString &scope, const QString &name) { sendQueueStream << static_cast<quint8>(0) << scope << name; addedToQueue(); }
    void leaveClient(const QString &scope, const QString &name) { sendQueueStream << static_cast<quint8>(5) << scope << name; addedToQueue(); }
    QHostAddress getAddress() const { return socket.peerAddress(); }

private slots:
    void readyRead();
    void disconnected() { server->disconnected(this); }
};

#endif // CLIENTCONNECTION_H
