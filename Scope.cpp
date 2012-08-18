#include "Scope.h"
#include "ClientConnection.h"

void Scope::getInitInfo(QDataStream &stream) const
{   
    stream << static_cast<quint32>(clients.length());
    for (const auto &client : clients) stream << client->nick();
    stream << static_cast<quint32>(scope_info.variables.size());
    for (const auto &item : scope_info.variables)
            stream << QString::fromStdString(item.first) << QString::fromStdString(item.second->toString());
    stream << static_cast<quint32>(scope_info.functions.size());
    for (const auto &item : scope_info.functions) {
        stream << QString::fromStdString(item.first.first) << static_cast<quint32>(item.second.first.size());
        for (const auto &arg : item.second.first) stream << QString::fromStdString(arg);
        stream << QString::fromStdString(item.second.second->toString());
    }
}

void Scope::removeClient(ClientConnection *client)
{
    if (clients.contains(client)) {
        clients.removeOne(client);
       for (const auto &it_client : clients) it_client->leaveClient(name, client->nick());
    }
}

void Scope::sendMsg(const QString &sender, const QString &msg)
{
    for (const auto &client : clients) client->sendMsg(name, sender, msg);
    try {
        std::unique_ptr<CAS::AbstractArithmetic> result = parser->parse(msg.toStdString());
        QString resultString = QString::fromStdString(result->eval(scope_info)->toString());
        if (result->getType() == CAS::AbstractArithmetic::ASSIGNMENT) {
            CAS::Assignment *ass(static_cast<CAS::Assignment*>(result.get()));
            if (ass->getFirstOp()->getType() == CAS::AbstractArithmetic::VARIABLE) {
                scope_info.variables[ass->getFirstOp()->toString()] =  std::shared_ptr<CAS::AbstractArithmetic>(ass->getSecondOp()->copy());
                for (const auto &client : clients)
                        client->newVariable(name, QString::fromStdString(ass->getFirstOp()->toString()), QString::fromStdString(ass->getSecondOp()->toString()));
                qDebug() << "NewVariable(" << name << ", " << QString::fromStdString(ass->getFirstOp()->toString()) << ", " << QString::fromStdString(ass->getSecondOp()->toString()) << ")";
                resultString = QString::fromStdString(ass->getSecondOp()->eval(scope_info)->toString());
            } else if (ass->getFirstOp()->getType() == CAS::AbstractArithmetic::FUNCTION) {
                const CAS::Function *func = static_cast<const CAS::Function*>(ass->getFirstOp());
                std::vector<std::string> argStrings;
                for (const auto &arg : func->getOperands()) argStrings.push_back(arg->toString());
                scope_info.functions[std::make_pair(func->getIdentifier(), argStrings.size())] = std::make_pair(argStrings, ass->getSecondOp()->copy());
                QStringList argQStrings;
                for (const auto &arg : argStrings) argQStrings.append(QString::fromStdString(arg));
                for (const auto &client : clients)
                        client->newFunction(name, QString::fromStdString(func->getIdentifier()), argQStrings, QString::fromStdString(ass->getSecondOp()->toString()));
                qDebug() << "NewFunction(" << name << ", " << QString::fromStdString(func->getIdentifier()) << ", " << argQStrings << ", " <<  QString::fromStdString(ass->getSecondOp()->toString()) << ")";
                resultString = QString::fromStdString(ass->getSecondOp()->eval(scope_info)->toString());
            }
        }
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", resultString);
    } catch (const char *) {
        for (const auto &client : clients) client->sendMsg(name, "Jarvis", "error bro");
    }
}

void Scope::addClient(ClientConnection *client)
{
    if (clients.contains(client)) throw 0;
    for (const auto &it_client : clients) it_client->enterClient(name, client->nick());
    clients.append(client);
}
