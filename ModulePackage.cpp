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
            auto module = std::make_shared<BinaryOperatorModule>(name, this);
            fileStream >> *module;
            modules.binaryOperators.append(module);
        } else if (type == "UnaryOperator") {
            auto module = std::make_shared<UnaryOperatorModule>(name, this);
            fileStream >> *module;
            modules.unaryOperators.append(module);
        } else if (type == "Function") {
            auto module = std::make_shared<FunctionModule>(name, this);
            fileStream >> *module;
            modules.functions.append(module);
        } else if (type == "TerminalExpression") {
            auto module = std::make_shared<TerminalExpressionModule>(name, this);
            fileStream >> *module;
            modules.terminalExpressions.append(module);
        } else if (type == "TerminalStatement") {
            auto module = std::make_shared<TerminalStatementModule>(name, this);
            fileStream >> *module;
            modules.terminalStatements.append(module);
        } else throw "failer";
        fileStream.skipWhiteSpace();
    }
}

QDataStream &operator<<(QDataStream &stream, const ModulePackage &pkg)
{
    stream << pkg.name() << pkg.getModules();
    return stream;
}
