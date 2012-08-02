#include "ClientConnection.h"
#include "JarvisServer.h"

ClientConnection::ClientConnection(JarvisServer *server) : stream(this), server(server), sendQueueStream(&sendQueue, QIODevice::WriteOnly)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

bool ClientConnection::receiveString(QString &dest)
{
    do {
    if (stringReceiveState == StringSize) {
        if (bytesAvailable() >= sizeof(quint32)) {
            QDataStream(peek(sizeof(quint32))) >> nextBlockSize;
            nextBlockSize += sizeof(quint32);
            stringReceiveState = String;
        } else return false;
    } else {
        if (bytesAvailable() >= nextBlockSize) {
            stream >> dest;
            stringReceiveState = StringSize;
            return true;
        } else return false;
    }
    } while (bytesAvailable());
}

void ClientConnection::readyRead()
{
    QString buffer_2;

    do {
        switch (state) {
        case Virgin:
            quint8 client_version;
            stream >> client_version;
            if (client_version == server->version()) {
                stream << static_cast<quint8>(1);
                state = AuthType;
            } else {
                stream << static_cast<quint8>(0) << server->version();
            }
            break;
        case AuthType:
            stream >> type;
            state = UserName;
            break;
        case UserName:
            if (receiveString(_name)) state = Password;
            else return;
            break;
        case Password:
            if (receiveString(buffer)) {
                quint8 success = server->login(_name, buffer);
                stream << success;
                if (success) {
                    for (const auto &scope : server->getScopeNames()) newScope(scope);
                    setLoop();
                } else state = AuthType;
            } else return;
            break;
        case Loop:
            stream >> type;
            switch (type) {
            case 0: state = EnterScope; break;
            case 1: state = LeaveScope; break;
            case 2: state = ClientMsgScope; break;
            }

            break;
        case EnterScope:
            if (receiveString(buffer)) {
                server->enterScope(this, buffer).getInitInfo(stream);
                setLoop();
            } else return;
            break;
        case LeaveScope:
            if (receiveString(buffer)) {
                server->leaveScope(this, buffer);
                setLoop();
            } else return;
            break;
        case ClientMsgScope:
            if (receiveString(buffer)) state = ClientMsg;
            else return;
            break;
        case ClientMsg:
            if (receiveString(buffer_2)) {
                server->msgToScope(this, buffer, buffer_2);
                setLoop();
            } else return;
            break;
        }
    } while (this->bytesAvailable());
}
