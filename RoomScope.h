#ifndef ROOMSCOPE_H
#define ROOMSCOPE_H

#include "lang/Scope.h"
#include <QString>
#include <QObject>
#include <QDataStream>
#include "lang/AbstractStatement.h"
#include <QFile>
#include "JarvisServer.h"
#include "lang/CFunctionBody.h"
#include <QLibrary>

class RoomScope : public QObject, public CAS::Scope
{
    Q_OBJECT

private:
    const JarvisServer &server;
    std::unique_ptr<CAS::Scope> libScope;

public:
    RoomScope(const JarvisServer &server) : server(server), libScope(make_unique<CAS::Scope>()) { CAS::Scope::parent = libScope.get(); }

    virtual void declareVar(CAS::TypeInfo type, std::string id);
    virtual void declareFunc(CAS::FunctionSignature sig, CAS::TypeInfo returnType);
    virtual void defineVar(const std::string &id, CAS::AbstractExpression::ExpressionP definition, bool recursion = false);
    virtual void defineFunc(const CAS::FunctionSignature &sig, std::vector<std::string> args, CAS::AbstractStatement::StatementP def);

    void load(const std::string &fileName);

    const VarDefs &getVars() const { return variables; }
    const FuncDefs &getFunc() const { return functions; }

    friend QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

signals:
    void declaredVar(const CAS::TypeInfo &type, const std::string &id);
    void definedVar(const std::string &id, const CAS::AbstractExpression::ExpressionP &definition);
    void declaredFunc(const CAS::FunctionSignature &sig, const CAS::TypeInfo &returnType);
    void definedFunc(const CAS::FunctionSignature &sig, const std::vector<std::string> &args, const CAS::AbstractStatement::StatementP &def);
};

QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

#endif // ROOMSCOPE_H
