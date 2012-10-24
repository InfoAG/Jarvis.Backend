#ifndef OPERATORSTATICS_H
#define OPERATORSTATICS_H

#include <memory>
#include <QString>

struct OperatorStatics {
    std::shared_ptr<QString> matches;
    unsigned int priority;
    bool needsParseForMatch;
};

#endif // OPERATORSTATICS_H
