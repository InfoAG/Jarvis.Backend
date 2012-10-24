#include "ModulePackage.h"

ModulePackage::ModulePackage(std::unique_ptr<QFile> file)
{
    name_ = QFileInfo(*file).baseName();
    file->open(QFile::ReadOnly);
    QTextStream fileStream(file.get());
    while (! fileStream.atEnd()) {
        QString name, type, buf;
        fileStream >> name >> buf;
        if (buf != ":") throw "failer";
        fileStream >> type >> buf;
        if (buf != "{") throw "failer{";
        if (type == "BinaryOperator") {
            BinaryOperatorModule module(name, this);
            fileStream >> module;
            modules.binaryOperators.append(module);
        } else if (type == "UnaryOperator") {
            UnaryOperatorModule module(name, this);
            fileStream >> module;
            modules.unaryOperators.append(module);
        } else if (type == "Function") {
            FunctionModule module(name, this);
            fileStream >> module;
            modules.functions.append(module);
        } else if (type == "Terminal") {
            TerminalModule module(name, this);
            fileStream >> module;
            modules.terminals.append(module);
        } else throw "failer";
        fileStream.skipWhiteSpace();
    }
}

QDataStream &operator<<(QDataStream &stream, const ModulePackage &pkg)
{
    stream << pkg.name() << pkg.getModules();
    return stream;
}
