#include "OperatorModule.h"

bool OperatorModule::matches(const std::string &input, size_t candidatePos, const ExpressionParser &parser) const
{
    if (interface.matches == nullptr) return input.at(candidatePos) == statics.matches->toStdString().front();
    else return interface.matches(input, candidatePos, parser);
}

unsigned int OperatorModule::priority() const
{
    if (interface.priority == nullptr) return statics.priority;
    else return interface.priority();
}

OperatorInterface::AssociativityType OperatorModule::associativity() const
{
    if (interface.associativity == nullptr) return statics.associativity;
    else return interface.associativity();
}

QDataStream &operator<<(QDataStream &stream, const OperatorModule &module)
{
    stream << module.name() << module.description();
    if (module.interface.matches == nullptr) stream << static_cast<quint8>(0) << *module.statics.matches;
    else stream << static_cast<quint8>(1);
    if (module.interface.priority == nullptr) stream << static_cast<quint8>(0) << module.statics.priority;
    else stream << static_cast<quint8>(1);
    if (module.interface.associativity == nullptr) stream << static_cast<quint8>(0) << static_cast<quint8>(module.statics.associativity);
    else stream << static_cast<quint8>(1);
    stream << static_cast<quint8>(module.statics.needsParseForMatch);
    return stream;
}
