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

QTextStream &operator>>(QTextStream &stream, FunctionModule &module)
{
    for (;;) {
        auto nextProperty = module.readProperty(stream);
        if (nextProperty.first == "description")
            module.description_ = nextProperty.second;
        else if (nextProperty.first == "lib")
            module.interface = ((FunctionInterface(*)())QLibrary::resolve(nextProperty.second, (module.name_ + "_jmodule").toLatin1().data()))();
        else if (nextProperty.first == "matches") {
            auto matchPair = nextProperty.second.split(',');
            module.statics.matches = std::make_shared<QPair<QString, unsigned int>>(matchPair.front(), matchPair.back().toUInt());
        } else if (nextProperty.first == "priority")
            module.statics.priority = nextProperty.second.toUInt();
        else if (nextProperty.first == QString()) break;
        else throw "nonono";
    }
    return stream;
}
