#include "FunctionModule.h"

bool FunctionModule::matches(const std::string &candidate, unsigned int argCount) const
{
    if (interface.matches == nullptr)
        return statics.matches->first.toStdString() == candidate && statics.matches->second == argCount;
    else return interface.matches(candidate, argCount);
}

unsigned int FunctionModule::priority() const
{
    if (interface.priority == nullptr) return statics.priority;
    else return interface.priority();
}

QDataStream &operator<<(QDataStream &stream, const FunctionModule &module)
{
    stream << module.name() << module.description();
    if (module.interface.matches == nullptr) stream << static_cast<quint8>(0) << *(module.statics.matches);
    else stream << static_cast<quint8>(1);
    if (module.interface.priority == nullptr) stream << static_cast<quint8>(0) << module.statics.priority;
    else stream << static_cast<quint8>(1);
    return stream;
}
