#include "OperatorInterface.h"
#include "FunctionInterface.h"
#include "Arithmetic/AbstractArithmetic.h"
#include "Arithmetic/Variable.h"
#include "Arithmetic/Assignment.h"
#include "Arithmetic/Function.h"
#include <memory>
#include "variables_global.h"

extern "C" {

std::unique_ptr<CAS::AbstractArithmetic> VARIABLESSHARED_EXPORT Variable_jmodule(const std::string &candidate)
{
    if (candidate.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Variable(candidate));
    else return nullptr;
}

FunctionInterface VARIABLESSHARED_EXPORT UserFunction_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &identifier, std::vector<std::unique_ptr<CAS::AbstractArithmetic>> &arguments) {
            return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Function(identifier, std::move(arguments)));
        };
    fi.matches = [](const std::string &candidate, unsigned int argCount) {
            return true;
        };
    return fi;
}

OperatorInterface VARIABLESSHARED_EXPORT Assignment_jmodule()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
            bool assignableFunction = false;
            if (first->type() == CAS::AbstractArithmetic::FUNCTION) {
                assignableFunction = true;
                for (const auto &arg : static_cast<CAS::Function*>(first.get())->getOperands())
                    if (arg->type() != CAS::AbstractArithmetic::VARIABLE) assignableFunction = false;
            }
            if (assignableFunction || first->type() == CAS::AbstractArithmetic::VARIABLE)
                return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Assignment(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
            else return std::unique_ptr<CAS::AbstractArithmetic>(nullptr);
        };
    return oi;
}

}
