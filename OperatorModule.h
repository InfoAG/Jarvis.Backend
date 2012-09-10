#ifndef OPERATORMODULE_H
#define OPERATORMODULE_H

#include "Arithmetic/AbstractArithmetic.h"
#include <string>
#include "OperatorInterface.h"
#include <QString>
#include "ParserModule.h"
#include <QDataStream>

class OperatorModule : public ParserModule
{
public:
    struct StaticInfo {
        std::shared_ptr<QString> matches;
        unsigned int priority;
        OperatorInterface::AssociativityType associativity;
        bool needsParseForMatch;
    };

private:
    OperatorInterface interface;
    StaticInfo statics;

public:
    OperatorModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const OperatorInterface &interface, const StaticInfo &statics) : ParserModule(name, description, parent_pkg), interface(interface), statics(statics) {}

    bool matches(std::string candidate) const;
    unsigned int priority() const;
    OperatorInterface::AssociativityType associativity() const;
    std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic> left, std::unique_ptr<CAS::AbstractArithmetic> right) const { return interface.parse(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(left), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(right)); }
    bool needsParseForMatch() const { return statics.needsParseForMatch; }

    friend QDataStream &operator<<(QDataStream &, const OperatorModule &);
};

QDataStream &operator<<(QDataStream &stream, const OperatorModule &module);

#endif //OPERATORMODULE_H
