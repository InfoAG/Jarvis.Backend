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
        QString resultString;
        if (result->type() == CAS::AbstractArithmetic::ASSIGNMENT) {
            if (static_cast<CAS::Assignment*>(result.get())->getFirstOp()->type() == CAS::AbstractArithmetic::VARIABLE) {
                std::unique_ptr<CAS::AbstractArithmetic> evalResult = result->eval(scope_info);
                resultString = QString::fromStdString(evalResult->toString());
                scope_info.variables[static_cast<CAS::Assignment*>(result.get())->getFirstOp()->toString()] = std::move(evalResult);
                for (const auto &client : clients)
                        client->newVariable(name, QString::fromStdString(static_cast<CAS::Assignment*>(result.get())->getFirstOp()->toString()), resultString);
                qDebug() << "NewVariable(" << name << ", " << QString::fromStdString(static_cast<CAS::Assignment*>(result.get())->getFirstOp()->toString()) << ", " << resultString << ")";
            } else if (static_cast<CAS::Assignment*>(result.get())->getFirstOp()->type() == CAS::AbstractArithmetic::FUNCTION) {
                //const CAS::Function *func = static_cast<const CAS::Function*>(static_cast<CAS::Assignment*>(result.get())->getFirstOp().get());
                std::vector<std::string> argStrings;
                CAS::ScopeInfo clearedFuncVars(scope_info);
                for (const auto &arg : static_cast<const CAS::Function*>(static_cast<CAS::Assignment*>(result.get())->getFirstOp().get())->getOperands()) {
                    std::string argString = arg->toString();
                    clearedFuncVars.variables.erase(argString);
                    argStrings.emplace_back(std::move(argString));
                }
                std::unique_ptr<CAS::AbstractArithmetic> evalResult = static_cast<CAS::Assignment*>(result.get())->getSecondOp()->eval(clearedFuncVars);
                resultString = QString::fromStdString(evalResult->toString());
                scope_info.functions[std::make_pair(static_cast<const CAS::Function*>(static_cast<CAS::Assignment*>(result.get())->getFirstOp().get())->getIdentifier(), argStrings.size())] = std::make_pair(argStrings, std::move(evalResult));
                QStringList argQStrings;
                for (const auto &arg : argStrings) argQStrings.append(QString::fromStdString(arg));
                for (const auto &client : clients)
                        client->newFunction(name, QString::fromStdString(static_cast<const CAS::Function*>(static_cast<CAS::Assignment*>(result.get())->getFirstOp().get())->getIdentifier()), argQStrings, resultString);
                qDebug() << "NewFunction(" << name << ", " << QString::fromStdString(static_cast<const CAS::Function*>(static_cast<CAS::Assignment*>(result.get())->getFirstOp().get())->getIdentifier()) << ", " << argQStrings << ", " <<  resultString << ")";
            }
        } else resultString = QString::fromStdString(result->eval(scope_info)->toString());
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
