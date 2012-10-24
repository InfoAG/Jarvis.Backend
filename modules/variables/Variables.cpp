#include "BinaryOperatorInterface.h"
#include "FunctionInterface.h"
#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Variable.h"
#include "Arithmetic/Assignment.h"
#include "Arithmetic/Function.h"
#include "Arithmetic/LazyEval.h"
#include <memory>
#include "variables_global.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT Variable_jmodule(const std::string &candidate)
{
    if (candidate.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
        return std::unique_ptr<CAS::AbstractExpression>(new CAS::Variable(candidate));
    else return nullptr;
}

FunctionInterface VARIABLESSHARED_EXPORT UserFunction_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &identifier, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return std::unique_ptr<CAS::AbstractExpression>(new CAS::Function(identifier, std::move(arguments)));
        };
    fi.matches = [](const std::string &, unsigned int) {
            return true;
        };
    return fi;
}

BinaryOperatorInterface VARIABLESSHARED_EXPORT Assignment_jmodule()
{
    BinaryOperatorInterface oi;
    oi.parse = [](std::unique_ptr<CAS::AbstractExpression> first, std::unique_ptr<CAS::AbstractExpression> second) {
            bool assignableFunction = false;
            if (typeid(*first) == typeid(CAS::Function)) {
                assignableFunction = true;
                for (const auto &arg : static_cast<CAS::Function*>(first.get())->getOperands())
                    if (typeid(*arg) != typeid(CAS::Variable)) assignableFunction = false;
            }
            if (assignableFunction || typeid(*first) == typeid(CAS::Variable))
                return std::unique_ptr<CAS::AbstractExpression>(new CAS::Assignment(std::forward<std::unique_ptr<CAS::AbstractExpression>>(first), std::forward<std::unique_ptr<CAS::AbstractExpression>>(second)));
            else return std::unique_ptr<CAS::AbstractExpression>(nullptr);
        };
    return oi;
}

FunctionInterface VARIABLESSHARED_EXPORT LazyEval_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::LazyEval>(std::move(arguments.front()));
        };
    return fi;
}

}
