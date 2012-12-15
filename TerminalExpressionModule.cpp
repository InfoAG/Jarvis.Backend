#include "TerminalExpressionModule.h"

QTextStream &operator>>(QTextStream &stream, TerminalExpressionModule &module)
{
    for (;;) {
        auto nextProperty = module.readProperty(stream);
        if (nextProperty.first == "description")
            module.description_ = nextProperty.second;
        else if (nextProperty.first == "lib") {
            module.lib.setFileName(nextProperty.second);
            module.parse_interface = (CAS::AbstractExpression::ExpressionP(*)(const std::string &, std::function<CAS::AbstractExpression::ExpressionP(std::string)>))module.lib.resolve((module.name_ + "_jmodule").toLatin1().data());
        } else if (nextProperty.first == QString()) break;
        else throw "nononot";
    }
    return stream;
}
