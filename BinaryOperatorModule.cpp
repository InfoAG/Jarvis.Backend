#include "BinaryOperatorModule.h"

std::pair<bool, size_t> BinaryOperatorModule::matches(const std::string &input, size_t candidatePos, const ExpressionParser &parser) const
{
    if (interface.matches == nullptr) {
        auto staticStr = statics.matches->toStdString();
        return std::make_pair(std::equal(staticStr.cbegin(), staticStr.cend(), input.cbegin() + candidatePos), statics.matches->size());
    } else return interface.matches(input, candidatePos, parser);
}

BinaryOperatorInterface::AssociativityType BinaryOperatorModule::associativity() const
{
    if (interface.associativity == nullptr) return statics.associativity;
    else return interface.associativity();
}

QDataStream &operator<<(QDataStream &stream, const BinaryOperatorModule &module)
{
    stream << static_cast<const OperatorModule<BinaryOperatorInterface, BinaryOperatorStatics>&>(module);
    stream << static_cast<quint8>((module.interface.associativity == nullptr) ? module.statics.associativity : 3);
    return stream;
}

QTextStream &operator>>(QTextStream &stream, BinaryOperatorModule &module)
{
    for (;;) {
        auto nextProperty = module.readProperty(stream);
        if (nextProperty.first == "description")
            module.description_ = nextProperty.second;
        else if (nextProperty.first == "lib")
            module.interface = ((BinaryOperatorInterface(*)())QLibrary::resolve(nextProperty.second, (module.name_ + "_jmodule").toLatin1().data()))();
        else if (nextProperty.first == "matches")
            module.statics.matches = std::make_shared<QString>(nextProperty.second);
        else if (nextProperty.first == "priority")
            module.statics.priority = nextProperty.second.toUInt();
        else if (nextProperty.first == "needsParseForMatch")
            module.statics.needsParseForMatch = nextProperty.second == "true" ? true : false;
        else if (nextProperty.first == "associativity")
            module.statics.associativity = nextProperty.second == "left" ? BinaryOperatorInterface::LEFT : BinaryOperatorInterface::RIGHT;
        else if (nextProperty.first == QString()) break;
        else throw "nononob";
    }
    return stream;
}
