#include "TerminalModule.h"

QTextStream &operator>>(QTextStream &stream, TerminalModule &module)
{
    for (;;) {
        auto nextProperty = module.readProperty(stream);
        if (nextProperty.first == "description")
            module.description_ = nextProperty.second;
        else if (nextProperty.first == "lib")
            module.parse_interface = (std::unique_ptr<CAS::AbstractExpression>(*)(const std::string &, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>))QLibrary::resolve(nextProperty.second, (module.name_ + "_jmodule").toLatin1().data());
        else if (nextProperty.first == QString()) break;
        else throw "nononot";
    }
    return stream;
}
