#include "BinaryOperatorInterface.h"
#include "FunctionInterface.h"
#include "Arithmetic/AbstractExpression.h"
#include "Arithmetic/Variable.h"
#include "Arithmetic/Assignment.h"
#include "Arithmetic/Function.h"
#include "Arithmetic/LazyEval.h"
#include <memory>
#include "variables_global.h"
#include "VariableDeclarationExpression.h"

extern "C" {

std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT Variable_jmodule(const std::string &candidate)
{
    if (candidate.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos)
        return make_unique<CAS::Variable>(candidate);
    else return nullptr;
}

FunctionInterface VARIABLESSHARED_EXPORT UserFunction_jmodule()
{
    FunctionInterface fi;
    fi.parse = [](const std::string &identifier, std::vector<std::unique_ptr<CAS::AbstractExpression>> &arguments) {
            return make_unique<CAS::Function>(identifier, std::move(arguments));
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
            if (assignableFunction || typeid(*first) == typeid(CAS::Variable) || typeid(*first) == typeid(VariableDeclarationExpression))
                return make_unique<CAS::Assignment>(std::move(first), std::move(second));
            else return std::unique_ptr<CAS::Assignment>(nullptr);
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

std::unique_ptr<CAS::AbstractExpression> VARIABLESSHARED_EXPORT VariableDeclaration_jmodule(const std::string &candidate, std::function<std::unique_ptr<CAS::AbstractExpression>(std::string)>)
{
    auto spaceIt = candidate.find_first_of(' ');
    std::string type = candidate.substr(0, spaceIt), id = candidate.substr(spaceIt + 1, std::string::npos);
    if (! (isalpha(id.front()) && std::find_if(id.begin(), id.end(), std::not1(std::pointer_to_unary_function<int, int>(isalnum))) == id.end()))
        return nullptr;
    if (type == "number") return make_unique<VariableDeclarationExpression>(CAS::AbstractExpression::NUMBER, std::move(id));
    else if (type == "bool") return make_unique<VariableDeclarationExpression>(CAS::AbstractExpression::BOOL, std::move(id));
    else if (type == "list") return make_unique<VariableDeclarationExpression>(CAS::AbstractExpression::LIST, std::move(id));
    else return nullptr;
}

}
