#include "ModulePackage.h"
//#include "Arithmetic/NumberArith.cpp"
//#include "Arithmetic/EvalInfo.h"
#include <QDebug>

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
    OperatorInterface opInter;
    FunctionInterface funcInter;
    QList<QByteArray> head;
    QString description;
    QLibrary lib;
    name_ = file->fileName();
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
                            modules.operators.append(OperatorModule(head.at(0), description, this, opInter));
                            break;
                        case FUNCTION:
                            modules.functions.append(FunctionModule(head.at(0), description, this, funcInter));
                            break;
                        case TERMINAL:
                            modules.terminals.append(TerminalModule(head.at(0), description, this, (std::unique_ptr<CAS::AbstractArithmetic>(*)(std::string))lib.resolve(head[0] + "_maker")));
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
                            auto matchfunc = [=](std::string candidate) {
                                return candidate == value.data();
                            };
                            if (type == OPERATOR) opInter.matches = matchfunc;
                            else if (type == FUNCTION) funcInter.matches = matchfunc;
                        } else if (type == OPERATOR && key == "priority") {
                            opInter.priority = [=] {
                                return value.toInt();
                            };
                        } else if (type == OPERATOR && key == "associativity") {
                            if (value == "left") opInter.associativity = [] {
                                    return OperatorInterface::LEFT;
                                };
                            else if (value == "right") opInter.associativity = [] {
                                    return OperatorInterface::RIGHT;
                                };
                        } else if (key == "lib") {
                            lib.setFileName(value);
                            std::string dbg = QString(head[0]).toStdString();
                            if (type == OPERATOR) opInter = ((OperatorInterface(*)())lib.resolve(head[0] + "_maker"))();
                            else if (type == FUNCTION) funcInter = ((FunctionInterface(*)())lib.resolve(head[0] + "_maker"))();
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
