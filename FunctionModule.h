#ifndef FUNCTIONMODULE_H
#define FUNCTIONMODULE_H

#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include "ParserModule.h"
#include "FunctionInterface.h"
#include "ParserModule.h"
#include <string>

class FunctionModule : public ParserModule
{
private:
    FunctionInterface interface;

public:
    FunctionModule(const QString &name, const QString &description, const ModulePackage &parent_pkg, const FunctionInterface &interface) : ParserModule(name, description, parent_pkg), interface(interface) {};

    inline bool matches(std::string candidate) const { return interface.matches(candidate); };
    inline std::unique_ptr<CAS::AbstractArithmetic> parse(std::unique_ptr<CAS::AbstractArithmetic> arg) const { return interface.parse(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(arg));};
};

#endif //FUNCTIONMODULE_H
