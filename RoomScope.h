#ifndef ROOMSCOPE_H
#define ROOMSCOPE_H

#include "Arithmetic/Scope.h"
#include <QString>
#include <QObject>
#include <QDataStream>
#include "Arithmetic/AbstractExpression.h"

class RoomScope : public QObject, public CAS::Scope
{
    Q_OBJECT

public:
    RoomScope() {}

    virtual void declareVar(CAS::AbstractExpression::ReturnType type, std::string id);
    virtual void declareFunc(CAS::FunctionSignature sig, CAS::AbstractExpression::ReturnType returnType);
    virtual void defineVar(const std::string &id, CAS::VariableDefinition var);
    virtual void defineFunc(const CAS::FunctionSignature &sig, CAS::FunctionDefinition def);

    const VarDefs &getVars() const { return variables; }
    const FuncDefs &getFunc() const { return functions; }

    friend QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

signals:
    void declaredVar(CAS::AbstractExpression::ReturnType type, const std::string &id);
    void definedVar(const std::string &id, const CAS::VariableDefinition &var);
    void declaredFunc(const CAS::FunctionSignature &sig, CAS::AbstractExpression::ReturnType returnType);
    void definedFunc(const CAS::FunctionSignature &sig, const CAS::FunctionDefinition &def);
};

QDataStream &operator<<(QDataStream &stream, const RoomScope &roomScope);

#endif // ROOMSCOPE_H
