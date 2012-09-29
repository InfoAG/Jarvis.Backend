#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QList>
#include <memory>
#include "ExpressionParser.h"
#include "RoomScope.h"
#include "Arithmetic/Assignment.h"

class ClientConnection;

class Room : QObject
{
    Q_OBJECT

private:
    QString name;
    ExpressionParser *parser;
    QList<ClientConnection *> clients;
    std::shared_ptr<RoomScope> roomScope;

public:
    Room() {};
    Room(const QString &name, ExpressionParser *parser);

    void getInitInfo(QDataStream &stream) const;
    void removeClient(ClientConnection *client);
    void sendMsg(const QString &sender, const QString &msg);
    void addClient(ClientConnection *client);
    bool hasClient(ClientConnection *candidate) { return clients.contains(candidate); }

private slots:
    void newVariable(const std::pair<std::string, CAS::Definition> &var);
    void newFunction(const std::pair<std::string, CAS::FunctionDefinition> &func);
};

#endif //ROOM_H
