#include "ParserModules.h"

QDataStream &operator<<(QDataStream &stream, const ParserModules &modules)
{
    stream << modules.terminals << modules.operators << modules.functions;
    return stream;
}
