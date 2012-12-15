#ifndef UNARYOPERATORMODULE_H
#define UNARYOPERATORMODULE_H

#include "UnaryOperatorStatics.h"
#include "OperatorModule.h"

class UnaryOperatorModule : public OperatorModule<UnaryOperatorInterface, UnaryOperatorStatics>
{
public:
    UnaryOperatorModule(const QString &name, const ModulePackage *parent_pkg) : OperatorModule(name, parent_pkg) {}
    UnaryOperatorModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const UnaryOperatorInterface &interface, const UnaryOperatorStatics &statics) : OperatorModule(name, description, parent_pkg, interface, statics) {}

    std::pair<bool, size_t> matches(const std::string &input) const;
    UnaryOperatorInterface::Alignment alignment() const;
    CAS::AbstractExpression::ExpressionP parse(CAS::AbstractExpression::ExpressionP operand) const { return interface.parse(std::move(operand)); }

    friend QDataStream &operator<<(QDataStream &, const UnaryOperatorModule &);
    friend QTextStream &operator>>(QTextStream &stream, UnaryOperatorModule &module);
};

#endif // UNARYOPERATORMODULE_H
