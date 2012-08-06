#include "ClientConnection.h"
#include "JarvisServer.h"

ClientConnection::ClientConnection(JarvisServer *server, int socketfd) : server(server), iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket), sendQueueStream(&sendQueue, QIODevice::WriteOnly)
{
    socket.setSocketDescriptor(socketfd);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ClientConnection::readyRead()
{
    QString buffer_2;

    streamBuf += socket.readAll();
    do {
        iStream.device()->reset();
        switch (connectionState) {
        case Virgin:
            if (pop_front() == server->version()) {
                oStream << static_cast<quint8>(1);
                connectionState = Auth;
            } else {
                oStream << static_cast<quint8>(0) << server->version();
            }
            break;
        case Auth:
            iStream >> type >> _name >> buffer;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                quint8 success = server->login(_name, buffer);
                oStream << success;
                if (success) {
                    for (const auto &scope : server->getScopeNames()) newScope(scope);
                    setLoop();
                } else connectionState = Auth;
            } else return;
            break;
        case Loop:
            switch (pop_front()) {
            case 0: connectionState = EnterScope; break;
            case 1: connectionState = LeaveScope; break;
            case 2: connectionState = ClientMsg; break;
            case 3: oStream << server->getParser()->getModulePkgs(); break;
            case 4: connectionState = UnloadPkg; break;
            case 5: connectionState = LoadPkg; break;
            }
            break;
        case EnterScope:
            iStream >> buffer;
            if (iStream.status() == QDataStream::Ok){
                resetStreamBuf();
                server->enterScope(this, buffer).getInitInfo(oStream);
                setLoop();
            } else return;
            break;
        case LeaveScope:
            iStream >> buffer;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                server->leaveScope(this, buffer);
                setLoop();
            } else return;
            break;
        case ClientMsg:
            iStream >> buffer >> buffer_2;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                server->msgToScope(this, buffer, buffer_2);
                setLoop();
            } else return;
            break;
        case UnloadPkg:
            iStream >> buffer;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                server->getParser()->unload(buffer);
                setLoop();
            } else return;
            break;
        case LoadPkg:
            iStream >> buffer;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                server->getParser()->load(buffer);
                setLoop();
            } else return;
            break;
        }
        if (socket.bytesAvailable()) streamBuf += socket.readAll();
    } while (! streamBuf.isEmpty());
}
