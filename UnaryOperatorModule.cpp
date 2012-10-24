#include "UnaryOperatorModule.h"

std::pair<bool, size_t> UnaryOperatorModule::matches(const std::string &input) const
{
    if (interface.matches == nullptr) {
        auto staticStr = statics.matches->toStdString();
        if (alignment() == UnaryOperatorInterface::PRE) return std::make_pair(std::equal(staticStr.cbegin(), staticStr.cend(), input.cbegin()), staticStr.size());
        else return std::make_pair(std::equal(staticStr.crend(), staticStr.crbegin(), input.crbegin()), staticStr.size());
    } else return interface.matches(input);
}

QDataStream &operator<<(QDataStream &stream, const UnaryOperatorModule &module)
{
    stream << static_cast<const OperatorModule<UnaryOperatorInterface, UnaryOperatorStatics>&>(module);
    stream << static_cast<quint8>((module.interface.alignment == nullptr) ? module.statics.alignment : 3);
    return stream;
}

QTextStream &operator>>(QTextStream &stream, UnaryOperatorModule &module)
{
    for (;;) {
        auto nextProperty = module.readProperty(stream);
        if (nextProperty.first == "description")
            module.description_ = nextProperty.second;
        else if (nextProperty.first == "lib")
            module.interface = ((UnaryOperatorInterface(*)())QLibrary::resolve(nextProperty.second, (module.name_ + "_jmodule").toLatin1().data()))();
        else if (nextProperty.first == "matches")
            module.statics.matches = std::make_shared<QString>(nextProperty.second);
        else if (nextProperty.first == "priority")
            module.statics.priority = nextProperty.second.toUInt();
        else if (nextProperty.first == "needsParseForMatch")
            module.statics.needsParseForMatch = nextProperty.second == "true" ? true : false;
        else if (nextProperty.first == "alignment")
            module.statics.alignment = nextProperty.second == "pre" ? UnaryOperatorInterface::PRE : UnaryOperatorInterface::POST;
        else if (nextProperty.first == QString()) break;
        else throw "nononou";
    }
    return stream;
}

UnaryOperatorInterface::Alignment UnaryOperatorModule::alignment() const
{
    if (interface.alignment == nullptr) return statics.alignment;
    else return interface.alignment();
}
