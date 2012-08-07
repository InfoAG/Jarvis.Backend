#include "ClientConnection.h"
#include "JarvisServer.h"

ClientConnection::ClientConnection(JarvisServer *server, int socketfd) : server(server), iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket)
{
    socket.setSocketDescriptor(socketfd);
    connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ClientConnection::readyRead()
{
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
        case Auth: {
                quint8 newSignup;
                QString pwd;
                iStream >> newSignup >> _name >> pwd;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    quint8 success = server->login(_name, pwd);
                    oStream << success;
                    if (success) {
                        oStream << server->getScopeNames() << server->getParser()->getModulePkgs();
                        connectionState = Loop;
                    } else connectionState = Auth;
                } else return;
            }
            break;
        case Loop:
            switch (pop_front()) {
            case 0: connectionState = EnterScope; break;
            case 1: connectionState = LeaveScope; break;
            case 2: connectionState = ClientMsg; break;
            case 3: connectionState = LoadPkg; break;
            case 4: connectionState = UnloadPkg; break;
            case 5: connectionState = DeleteScope; break;
            }
            break;
        case EnterScope: {
                quint8 requestID;
                QString scope;
                iStream >> requestID >> scope;
                if (iStream.status() == QDataStream::Ok){
                    resetStreamBuf();
                    oStream << static_cast<quint8>(8) << requestID;
                    server->enterScope(this, scope).getInitInfo(oStream);
                    connectionState = Loop;
                } else return;
            }
            break;
        case LeaveScope: {
                QString scope;
                iStream >> scope;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    server->leaveScope(this, scope);
                    connectionState = Loop;
                } else return;
            }
            break;
        case ClientMsg: {
                QString scope, msg;
                iStream >> scope >> msg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    server->msgToScope(this, scope, msg);
                    connectionState = Loop;
                } else return;
            }
            break;
        case UnloadPkg: {
                QString pkg;
                iStream >> pkg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    server->unload(pkg);
                    connectionState = Loop;
                } else return;
            }
            break;
        case LoadPkg: {
                QString pkg;
                iStream >> pkg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    server->load(pkg);
                    connectionState = Loop;
                } else return;
            }
            break;
        case DeleteScope: {
                QString scope;
                iStream >> scope;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    server->deleteScope(scope);
                }
            }
        }
        if (socket.bytesAvailable()) streamBuf += socket.readAll();
    } while (! streamBuf.isEmpty());
}
