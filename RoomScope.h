#ifndef ROOMSCOPE_H
#define ROOMSCOPE_H

#include "Arithmetic/Scope.h"
#include <QString>
#include <QObject>
#include "Arithmetic/AbstractExpression.h"

class RoomScope : public QObject, public CAS::Scope
{
    Q_OBJECT

public:
    RoomScope() {}

    virtual void assignVar(const std::pair<std::string, CAS::Definition> &var);
    virtual void assignFunc(const std::pair<std::string, CAS::FunctionDefinition> &func);

    const VarDefs &getVars() const { return variables; }
    const FuncDefs &getFunc() const { return functions; }

signals:
    void newVariable(const std::pair<std::string, CAS::Definition> &);
    void newFunction(const std::pair<std::string, CAS::FunctionDefinition> &);
};

#endif // ROOMSCOPE_H
