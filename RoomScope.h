#ifndef ROOMSCOPE_H
#define ROOMSCOPE_H

#include "expression/Scope.h"
#include <QString>
#include <QObject>
#include <QDataStream>
#include "expression/AbstractExpression.h"
#include <QFile>
#include "JarvisServer.h"
#include "expression/CFunctionBody.h"
#include <QLibrary>

class RoomScope : public QObject, public CAS::Scope
{
    Q_OBJECT

private:
    const JarvisServer &server;

public:
    RoomScope(const JarvisServer &server) : server(server) {}

    virtual void declareVar(CAS::TypeInfo type, std::string id);
    virtual void declareFunc(CAS::FunctionSignature sig, CAS::TypeInfo returnType);
    virtual void defineVar(const std::string &id, CAS::VariableDefinition var);
    virtual void defineFunc(const CAS::FunctionSignature &sig, CAS::FunctionDefinition def);

    void load(const std::string &fileName);

    const VarDefs &getVars() const { return variables; }
    const FuncDefs &getFunc() const { return functions; }

    friend QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

signals:
    void declaredVar(const CAS::TypeInfo &type, const std::string &id);
    void definedVar(const std::string &id, const CAS::VariableDefinition &var);
    void declaredFunc(const CAS::FunctionSignature &sig, const CAS::TypeInfo &returnType);
    void definedFunc(const CAS::FunctionSignature &sig, const CAS::FunctionDefinition &def);
};

QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

#endif // ROOMSCOPE_H
