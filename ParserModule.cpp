#include "ParserModule.h"

QDataStream &operator<<(QDataStream &stream, const ParserModule &module)
{
    stream << module.name << module.description;
    return stream;
}
