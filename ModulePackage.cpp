#include "ModulePackage.h"

ModulePackage::ModulePackage(QFile *file)
{
    enum {
        HEAD,
        BODY
    } state = HEAD;

    enum {
        OPERATOR,
        FUNCTION,
        TERMINAL
    } type;

    if (file->open(QFile::ReadOnly)) {
        while (!file->atEnd()) {
            QByteArray line = file->readLine().trimmed();
            if (! line.isEmpty()) {
                switch (state) {
                case HEAD:
                    QList<QByteArray> head = line.split(':');
                    head.at(1) = head.at(1).trimmed().toUpper();
                    if (head.at(1) == "OPERATOR") {
                        type = OPERATOR;
                        OperatorInterface *opInter = new OperatorInterface;
                    } else if (head.at(1) == "FUNCTION") {
                        type = FUNCTION;
                        FunctionInterface *funcInter = new FunctionInterface;
                    } else if (head.at(1) == "TERMINAL") {
                        type = TERMINAL;
                    }
                    break;
                case BODY:
                }
            }
        }
    }
}
