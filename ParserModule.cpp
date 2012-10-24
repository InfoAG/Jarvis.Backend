#include "ParserModule.h"

QDataStream &operator<<(QDataStream &stream, const ParserModule &module)
{
    stream << module.name() << module.description();
    return stream;
}

std::pair<QString, QString> ParserModule::readProperty(QTextStream &stream)
{
    QString first, second;
    stream >> first;
    if (first == "}") return std::pair<QString, QString>();
    else {
        if (first.right(1) == ":") first.chop(1);
        else {
            stream >> second;
            if (second != ":") throw "nonono";
            second.clear();
        }
        while (true) {
            QString c = stream.read(1);
            if (c == ";" || c == "\n") break;
            else second += c;
        }
        second = second.trimmed();
        return std::make_pair(first, second);
    }
}
