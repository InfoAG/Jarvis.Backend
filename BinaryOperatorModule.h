#ifndef BINARYOPERATORMODULE_H
#define BINARYOPERATORMODULE_H

#include "OperatorModule.h"
#include "BinaryOperatorStatics.h"

class BinaryOperatorModule : public OperatorModule<BinaryOperatorInterface, BinaryOperatorStatics>
{
public:
    BinaryOperatorModule(const QString &name, const ModulePackage *parent_pkg) : OperatorModule(name, parent_pkg) {}
    BinaryOperatorModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const BinaryOperatorInterface &interface, const BinaryOperatorStatics &statics) : OperatorModule(name, description, parent_pkg, interface, statics) {}

    std::pair<bool, size_t> matches(const std::string &input, size_t candidatePos, const ExpressionParser &parser) const;
    BinaryOperatorInterface::AssociativityType associativity() const;
    std::unique_ptr<CAS::AbstractExpression> parse(std::unique_ptr<CAS::AbstractExpression> left, std::unique_ptr<CAS::AbstractExpression> right) const { return interface.parse(std::move(left), std::move(right)); }

    friend QDataStream &operator<<(QDataStream &, const BinaryOperatorModule &);
    friend QTextStream &operator>>(QTextStream &stream, BinaryOperatorModule &module);
};

//QTextStream &operator>>(QTextStream &stream, BinaryOperatorModule &module);

#endif // BINARYOPERATORMODULE_H
