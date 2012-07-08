#include "ClientConnection.h"

ClientConnection::ClientConnection() : stream(this)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ClientConnection::readyRead()
{
    switch (state) {
    case VIRGIN:
        quint32 client_version;
        stream >> client_version;
        if (client_version == server->settings.value("Version").toUInt()) {
            stream << (bool)1;
            state = AUTHTYPE;
        } else {
            stream << (bool)0 << server->settings.value("Version").toUInt();
        }
        break;
    case AUTHTYPE:
        bool newSignup;
        stream >> newSignup;
        state = USRSIZE;
        break;
    case USRSIZE:
        if (bytesAvailable() < sizeof(quint32)) return;
        else {
            stream >> nextBlockSize;
            state = USRSTRING;
        }
        break;
    case USRSTRING:
        if (bytesAvailable() < nextBlockSize) return;
        else {
            name = read(nextBlockSize);
        }
        break;
    case PWDSIZE:
        if (bytesAvailable() < sizeof(quint32)) return;
        else {
            stream >> nextBlockSize;
            state = PWDSTRING;
        }
        break;
    case PWDSTRING:
        if (bytesAvailable() < nextBlockSize) return;
        else {
            pwd = read(nextBlockSize);
        }
        break;
    case LOOP:
        break;
    }
}
