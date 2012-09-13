#include "ModulePackage.h"

ModulePackage::ModulePackage(std::unique_ptr<QFile> file)
{
    enum {
        HEAD,
        BRACE,
        BODY
    } state = HEAD;

    enum {
        OPERATOR,
        FUNCTION,
        TERMINAL
    } type;
    OperatorModule::StaticInfo opStatics;
    FunctionModule::StaticInfo funcStatics;
    OperatorInterface opInter;
    FunctionInterface funcInter;
    QList<QByteArray> head;
    QString description;
    QLibrary lib;
    name_ = QFileInfo(*file).baseName();
    if (file->open(QFile::ReadOnly)) {
        while (!file->atEnd()) {
            QByteArray line = file->readLine().trimmed();
            if (! line.isEmpty()) {
                switch (state) {
                case HEAD:
                    head = line.split(':');
                    head[0] = head.at(0).trimmed();
                    head[1] = head.at(1).trimmed().toUpper();
                    if (head.at(1) == "OPERATOR") {
                        type = OPERATOR;
                        //opInter = new OperatorInterface;
                    } else if (head.at(1) == "FUNCTION") {
                        type = FUNCTION;
                        //funcInter = new FunctionInterface;
                    } else if (head.at(1) == "TERMINAL") {
                        type = TERMINAL;
                    }
                    state = BRACE;
                    break;
                case BRACE:
                    if (line == "{") state = BODY;
                    break;
                case BODY:
                    if (line == "}") {
                        switch (type) {
                        case OPERATOR:
                            modules.operators.append(OperatorModule(head.at(0), description, this, opInter, opStatics));
                            break;
                        case FUNCTION:
                            modules.functions.append(FunctionModule(head.at(0), description, this, funcInter, funcStatics));
                            break;
                        case TERMINAL:
                            modules.terminals.append(TerminalModule(head.at(0), description, this, (std::unique_ptr<CAS::AbstractArithmetic>(*)(const std::string &, std::function<std::unique_ptr<CAS::AbstractArithmetic>(std::string)>))lib.resolve(head[0] + "_jmodule")));
                            break;
                        }
                        state = HEAD;
                    } else {
                        auto colonPos = line.indexOf(':');
                        auto key = line.left(colonPos).trimmed();
                        auto value = line.right(line.length() - colonPos - 2).trimmed();
                        if (key == "description") {
                            description = value;
                        } else if (key == "matches") {
                            if (type == OPERATOR) {
                                opStatics.matches = std::make_shared<QString>(value);
                            } else if (type == FUNCTION) {
                                QList<QByteArray> matchParts = value.split(',');
                                funcStatics.matches = std::make_shared<QPair<QString, unsigned int>>(QString(matchParts.at(0)), matchParts.at(1).toUInt());
                            }
                        } else if (key == "priority") {
                            if (type == OPERATOR) opStatics.priority = value.toUInt();
                            else if (type == FUNCTION) funcStatics.priority = value.toUInt();
                        } else if (key == "needsParseForMatch" && type == OPERATOR)
                            opStatics.needsParseForMatch = (value == "true") ? true : false;
                        else if (type == OPERATOR && key == "associativity") {
                            if (value == "left") opStatics.associativity = OperatorInterface::LEFT;
                            else if (value == "right") opStatics.associativity = OperatorInterface::RIGHT;
                        } else if (key == "lib") {
                            lib.setFileName(value);
                            std::string dbg = QString(head[0]).toStdString();
                            if (type == OPERATOR) opInter = ((OperatorInterface(*)())lib.resolve(head[0] + "_jmodule"))();
                            else if (type == FUNCTION) funcInter = ((FunctionInterface(*)())lib.resolve(head[0] + "_jmodule"))();
                        }
                    }
                }
            }
        }
    }
}

QDataStream &operator<<(QDataStream &stream, const ModulePackage &pkg)
{
    stream << pkg.name() << pkg.getModules();
    return stream;
}
