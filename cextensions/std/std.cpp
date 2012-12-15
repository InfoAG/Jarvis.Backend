#include "std_global.h"
#include "lang/AbstractExpression.h"
#include "lang/NumberValue.h"
#include "lang/Function.h"
#include "lang/List.h"
#include "lang/Addition.h"
#include "lang/BinaryMultiplication.h"
#include "lang/Variable.h"
#include <math.h>

extern "C" {

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT max_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption)
{
    try {
        auto argList = *static_cast<CAS::List*>(args.front()->eval(scope, load, print, CAS::AbstractExpression::EAGER).get());
        if (argList.getOperands().empty()) return nullptr;
        else {
            double maxVal = static_cast<CAS::NumberValue*>(argList.getOperands().front().get())->getValue();
            for (auto it = argList.getOperands().cbegin() + 1; it != argList.getOperands().cend(); ++it)
                if (static_cast<CAS::NumberValue*>(it->get())->getValue() > maxVal) maxVal = static_cast<CAS::NumberValue*>(it->get())->getValue();
            return make_unique<CAS::NumberValue>(maxVal);
        }
    } catch (CAS::JarvisException &) { return nullptr; }
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT min_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption)
{
    try {
        auto argList = *static_cast<CAS::List*>(args.front()->eval(scope, load, print, CAS::AbstractExpression::EAGER).get());
        if (argList.getOperands().empty()) return nullptr;
        else {
            double minVal = static_cast<CAS::NumberValue*>(argList.getOperands().front().get())->getValue();
            for (auto it = argList.getOperands().cbegin() + 1; it != argList.getOperands().cend(); ++it)
                if (static_cast<CAS::NumberValue*>(it->get())->getValue() < minVal) minVal = static_cast<CAS::NumberValue*>(it->get())->getValue();
            return make_unique<CAS::NumberValue>(minVal);
        }
    } catch (CAS::JarvisException &) { return nullptr; }
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT range_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    if (typeid(*(args.front())) == typeid(CAS::NumberValue) && typeid(*(args[1])) == typeid(CAS::NumberValue) && typeid(*(args.back())) == typeid(CAS::NumberValue)) {
        CAS::AbstractExpression::Expressions result;
        for (double i = std::move(static_cast<CAS::NumberValue*>(args.front().get())->getValue()); i <= static_cast<CAS::NumberValue*>(args[1].get())->getValue(); i += static_cast<CAS::NumberValue*>(args.back().get())->getValue())
            result.emplace_back(make_unique<CAS::NumberValue>(std::move(i)));
        return CAS::List(std::move(result)).eval(scope, load, print, execOption);
    } else return nullptr;
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT sum_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    return static_cast<CAS::List*>(args.front().get())->sum()->eval(scope, load, print, execOption);
}

double factorial(double n)
{
    double result = 1.0;
    if (n < 0) while (n) result *= n++;
    else while (n) result *= n--;
    return result;
}

inline double doBinom(double n, double k)
{
    return factorial(n) / (factorial(n-k) * factorial(k));
}

inline double doBinomPDF(double n, double p, double x)
{
    return doBinom(n, x) * pow(p, x) * pow(1 - p, n - x);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT binom_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue) || typeid(*(args.back())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(doBinom(static_cast<CAS::NumberValue*>(args.front().get())->getValue(), static_cast<CAS::NumberValue*>(args.back().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT binompdf_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue) || typeid(*(args.at(1))) != typeid(CAS::NumberValue) || typeid(*(args.back())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(doBinomPDF(static_cast<CAS::NumberValue*>(args.front().get())->getValue(), static_cast<CAS::NumberValue*>(args.at(1).get())->getValue(), static_cast<CAS::NumberValue*>(args.back().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT binomcdf_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue) || typeid(*(args.at(1))) != typeid(CAS::NumberValue) || typeid(*(args.back())) != typeid(CAS::NumberValue)) return nullptr;
    double result = 0;
    unsigned int start = static_cast<CAS::NumberValue*>(args.back().get())->getValue();
    do {
        result += doBinomPDF(static_cast<CAS::NumberValue*>(args.front().get())->getValue(), static_cast<CAS::NumberValue*>(args.at(1).get())->getValue(), start);
    } while (start--);
    return make_unique<CAS::NumberValue>(result);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT crossProduct_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    if (typeid(*(args.front())) != typeid(CAS::Vector) || typeid(*(args.back())) != typeid(CAS::Vector)) return nullptr;
    return CAS::Vector(make_unique<CAS::Subtraction>(make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getY()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getZ()->copyEx()), make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getZ()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getY()->copyEx())), make_unique<CAS::Subtraction>(make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getZ()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getX()->copyEx()), make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getX()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getZ()->copyEx())), make_unique<CAS::Subtraction>(make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getX()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getY()->copyEx()), make_unique<CAS::BinaryMultiplication>(static_cast<const CAS::Vector*>(args.front().get())->getY()->copyEx(), static_cast<const CAS::Vector*>(args.back().get())->getX()->copyEx()))).eval(scope, load, print, execOption);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT x_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::Vector)) return nullptr;
    return static_cast<const CAS::Vector*>(args.front().get())->getX()->copyEx();
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT y_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::Vector)) return nullptr;
    return static_cast<const CAS::Vector*>(args.front().get())->getY()->copyEx();
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT z_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::Vector)) return nullptr;
    return static_cast<const CAS::Vector*>(args.front().get())->getZ()->copyEx();
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT sqrt_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(sqrt(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT length_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    CAS::AbstractExpression::Expressions xArgs, yArgs, zArgs, addArgs, sqrtArgs;
    xArgs.emplace_back(args.front()->copyEx());
    yArgs.emplace_back(args.front()->copyEx());
    zArgs.emplace_back(args.front()->copyEx());
    addArgs.emplace_back(make_unique<CAS::Exponentiation>(make_unique<CAS::Function>("x", std::move(xArgs)), make_unique<CAS::NumberValue>(2)));
    addArgs.emplace_back(make_unique<CAS::Exponentiation>(make_unique<CAS::Function>("y", std::move(yArgs)), make_unique<CAS::NumberValue>(2)));
    addArgs.emplace_back(make_unique<CAS::Exponentiation>(make_unique<CAS::Function>("z", std::move(zArgs)), make_unique<CAS::NumberValue>(2)));
    sqrtArgs.emplace_back(make_unique<CAS::Addition>(std::move(addArgs)));
    return CAS::Function("sqrt", std::move(sqrtArgs)).eval(scope, load, print, execOption);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT sin_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(sin(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT cos_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(cos(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT tan_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(tan(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT asin_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(asin(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT acos_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(acos(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT atan_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &, const std::function<void(const std::string &)> &, CAS::AbstractExpression::ExecOption)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberValue)) return nullptr;
    return make_unique<CAS::NumberValue>(atan(static_cast<const CAS::NumberValue*>(args.front().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT angle_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    CAS::AbstractExpression::Expressions acosArgs, aLengthArgs, bLengthArgs;
    aLengthArgs.emplace_back(args.front()->copyEx());
    bLengthArgs.emplace_back(args.back()->copyEx());
    acosArgs.emplace_back(make_unique<CAS::Division>(make_unique<CAS::BinaryMultiplication>(args.front()->copyEx(), args.back()->copyEx()), make_unique<CAS::BinaryMultiplication>(make_unique<CAS::Function>("length", std::move(aLengthArgs)), make_unique<CAS::Function>("length", std::move(bLengthArgs)))));
    return CAS::Function("acos", std::move(acosArgs)).eval(scope, load, print, execOption);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT zero2d_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    CAS::AbstractExpression::Expressions listOps, sqrtOps, multOps, sqrtOps2, multOps2;
    multOps.emplace_back(make_unique<CAS::NumberValue>(4));
    multOps.emplace_back(args.front()->copyEx());
    multOps.emplace_back(args.back()->copyEx());
    multOps2.emplace_back(make_unique<CAS::NumberValue>(4));
    multOps2.emplace_back(args.front()->copyEx());
    multOps2.emplace_back(args.back()->copyEx());
    sqrtOps.emplace_back(make_unique<CAS::Subtraction>(make_unique<CAS::Exponentiation>(args.at(1)->copyEx(), make_unique<CAS::NumberValue>(2)), make_unique<CAS::LevelMultiplication>(std::move(multOps))));
    sqrtOps2.emplace_back(make_unique<CAS::Subtraction>(make_unique<CAS::Exponentiation>(args.at(1)->copyEx(), make_unique<CAS::NumberValue>(2)), make_unique<CAS::LevelMultiplication>(std::move(multOps2))));
    listOps.emplace_back(make_unique<CAS::Division>(make_unique<CAS::Subtraction>(make_unique<CAS::Function>("sqrt", std::move(sqrtOps)), args.at(1)->copyEx()), make_unique<CAS::LevelMultiplication>(make_unique<CAS::NumberValue>(2), args.front()->copyEx())));
    listOps.emplace_back(make_unique<CAS::Division>(make_unique<CAS::Subtraction>(make_unique<CAS::LevelMultiplication>(make_unique<CAS::NumberValue>(-1), make_unique<CAS::Function>("sqrt", std::move(sqrtOps2))), args.at(1)->copyEx()), make_unique<CAS::LevelMultiplication>(make_unique<CAS::NumberValue>(2), args.front()->copyEx())));
    return CAS::List(std::move(listOps)).eval(scope, load, print, execOption);
}

CAS::AbstractExpression::ExpressionP STDSHARED_EXPORT differentiate_jce(const CAS::AbstractExpression::Expressions &args, CAS::Scope &scope, const CAS::AbstractStatement::LoadFunc &load, const CAS::AbstractStatement::PrintFunc &print, CAS::AbstractExpression::ExecOption execOption)
{
    if (typeid(*(args.back())) != typeid(CAS::Variable)) return nullptr;
    return args.front()->differentiate(static_cast<const CAS::Variable*>(args.back().get())->getIdentifier())->execute(scope, load, print, execOption);
}

}
