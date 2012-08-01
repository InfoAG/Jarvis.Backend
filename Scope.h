#ifndef SCOPE_H
#define SCOPE_H

#include <QString>
#include <QList>
#include <memory>
#include "../Framework/Arithmetic/EvalInfo.h"
#include "ClientConnection.h"

class Scope
{
private:
    QString name;
    CAS::EvalInfo scope_info;
    QList<ClientConnection *> clients;

public:
    Scope() {};
    Scope(const QString &name) : name(name) {};

    void getInitInfo(QDataStream &stream) const;
    void removeClient(ClientConnection *client);
    void sendMsg(const QString &sender, const QString &msg) const;
    void addClient(ClientConnection *client);
};

#endif //SCOPE_H
