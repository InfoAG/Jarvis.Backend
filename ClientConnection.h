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
        LoadPkg,
        DeleteScope
    } connectionState = Virgin;

    QTcpSocket socket;
    JarvisServer *server;
    QDataStream iStream, oStream;
    QString _name;
    QByteArray streamBuf;

    quint8 pop_front() { quint8 result(streamBuf.at(0)); streamBuf.remove(0, 1); return result; }
    void resetStreamBuf() { streamBuf.remove(0, iStream.device()->pos()); }

public:
    ClientConnection(JarvisServer *server, int socketfd);

    QString name() const { return _name; };
    void sendMsg(const QString &scope, const QString &sender, const QString &msg) { oStream << static_cast<quint8>(4) << scope << sender << msg; };
    void newScope(const QString &name) { oStream << static_cast<quint8>(3) << name; };
    void enterClient(const QString &scope, const QString &name) { oStream << static_cast<quint8>(0) << scope << name; }
    void leaveClient(const QString &scope, const QString &name) { oStream << static_cast<quint8>(5) << scope << name; }
    void deleteScope(const QString &scope) { oStream << static_cast<quint8>(9) << scope; }
    QHostAddress getAddress() const { return socket.peerAddress(); }

private slots:
    void readyRead();
    void disconnected() { server->disconnected(this); }
};

#endif // CLIENTCONNECTION_H
