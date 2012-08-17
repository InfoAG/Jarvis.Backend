#include "OperatorInterface.h"
#include "Arithmetic/AbstractArithmetic.h"
#include "Arithmetic/Variable.h"
#include "Arithmetic/Assignment.h"
#include <memory>
#include "variables_global.h"

extern "C" {

std::unique_ptr<CAS::AbstractArithmetic> VARIABLESSHARED_EXPORT Variable_maker(std::string candidate)
{
    if (candidate.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
        return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Variable(candidate));
    else return nullptr;
}

OperatorInterface VARIABLESSHARED_EXPORT Assignment_maker()
{
    OperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractArithmetic> first, std::unique_ptr<CAS::AbstractArithmetic> second) {
            /*bool assignableFunction = false;
            if (first->getType() == CAS::AbstractArithmetic::FUNCTION) {
                assignableFunction = true;
                std::unique_ptr<CAS::Function> func(static_cast<CAS::Function*>(first.release()));
                for (const auto &arg : func.getArguments())
                    if (arg.getType() != CAS::AbstractArithmetic::VARIABLE) assignableFunction = false;
            }*/
            if (first->getType() == CAS::AbstractArithmetic::VARIABLE)
                return std::unique_ptr<CAS::AbstractArithmetic>(new CAS::Assignment(std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(first), std::forward<std::unique_ptr<CAS::AbstractArithmetic>>(second)));
            else return std::unique_ptr<CAS::AbstractArithmetic>(nullptr);
        };
    return oi;
}

}
