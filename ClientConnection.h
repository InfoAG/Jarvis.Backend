#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include <QString>

class JarvisServer;

class ClientConnection : public QTcpSocket
{
    Q_OBJECT

private:
    enum {
        Virgin,
        AuthType,
        UserName,
        Password,
        Loop,
        EnterScope,
        LeaveScope,
        ClientMsgScope,
        ClientMsg
    } state = Virgin;

    enum {
        StringSize,
        String
    } stringReceiveState = StringSize;

    QDataStream stream;
    JarvisServer *server;
    QString _name;
    QString buffer;
    quint32 nextBlockSize;
    quint8 type;
    QByteArray sendQueue;
    QDataStream sendQueueStream;

    void setLoop() { state = Loop; dispatchQueue(); }
    void addedToQueue() { if (state == Loop) dispatchQueue(); }
    void dispatchQueue() { write(sendQueue); sendQueue.clear(); sendQueueStream.device()->reset(); }
    bool receiveString(QString &dest);

public:
    ClientConnection(JarvisServer *server);

    QString name() const { return _name; };
    void sendMsg(const QString &scope, const QString &sender, const QString &msg) { sendQueueStream << static_cast<quint8>(4) << scope << sender << msg; addedToQueue(); };
    void newScope(const QString &name) { sendQueueStream << static_cast<quint8>(3) << name; addedToQueue(); };
    void enterClient(const QString &scope, const QString &name) { sendQueueStream << static_cast<quint8>(0) << scope << name; addedToQueue(); }
    void leaveClient(const QString &scope, const QString &name) { sendQueueStream << static_cast<quint8>(5) << scope << name; addedToQueue(); }

private slots:
    void readyRead();
    void disconnected() {};
};

#endif // CLIENTCONNECTION_H
