#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QDataStream>
#include "JarvisServer.h"
#include <QString>

class ClientConnection : public QTcpSocket
{
    Q_OBJECT

public:
    ClientConnection();

private:
    enum State {
        VIRGIN,
        AUTHTYPE,
        USRSIZE,
        USRSTRING,
        PWDSIZE,
        PWDSTRING,
        LOOP
    } state;

    QDataStream stream;
    JarvisServer *server;
    QString name;
    QString pwd;
    quint32 nextBlockSize;


private slots:
    void readyRead();
    void disconnected() {};
};

#endif // CLIENTCONNECTION_H
