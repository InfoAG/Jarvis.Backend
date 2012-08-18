#ifndef FUNCTIONMODULE_H
#define FUNCTIONMODULE_H

#include "Arithmetic/AbstractArithmetic.h"
#include <string>
#include "FunctionInterface.h"

class FunctionModule : public ParserModule
{
private:
    FunctionInterface interface;

public:
    FunctionModule(const QString &name, const QString &description, const ModulePackage *parent_pkg, const FunctionInterface &interface) : ParserModule(name, description, parent_pkg), interface(interface) {}

    inline bool matches(const std::string &candidate, unsigned int argCount) const { return interface.matches(candidate, argCount); }
    inline std::unique_ptr<CAS::AbstractArithmetic> parse(const std::string &identifier, const std::vector<std::shared_ptr<CAS::AbstractArithmetic>> &args) const { return interface.parse(identifier, args);}
    inline unsigned int priority() const { return interface.priority(); }
};

#endif //FUNCTIONMODULE_H
