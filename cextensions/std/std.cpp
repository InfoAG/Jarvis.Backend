#include "std_global.h"
#include "expression/AbstractExpression.h"
#include "expression/NumberArith.h"
#include "expression/Function.h"
#include "expression/List.h"
#include "expression/Addition.h"

extern "C" {

CAS::AbstractExpression::ExpressionP max(const CAS::AbstractExpression::Operands &args, CAS::Scope &, const std::function<void(const std::string &)> &, bool, bool)
{
    if (typeid(*(args.front())) == typeid(CAS::NumberArith) && typeid(*(args.back())) == typeid(CAS::NumberArith)) {
        if (static_cast<CAS::NumberArith*>(args.front().get())->getValue() > static_cast<CAS::NumberArith*>(args.back().get())->getValue())
            return args.front()->copy();
        else return args.back()->copy();
    } else return nullptr;
}

CAS::AbstractExpression::ExpressionP min(const CAS::AbstractExpression::Operands &args, CAS::Scope &, const std::function<void(const std::string &)> &, bool, bool)
{
    if (typeid(*(args.front())) == typeid(CAS::NumberArith) && typeid(*(args.back())) == typeid(CAS::NumberArith)) {
        if (static_cast<CAS::NumberArith*>(args.front().get())->getValue() < static_cast<CAS::NumberArith*>(args.back().get())->getValue())
            return args.front()->copy();
        else return args.back()->copy();
    } else return nullptr;
}

CAS::AbstractExpression::ExpressionP range(const CAS::AbstractExpression::Operands &args, CAS::Scope &scope, const std::function<void(const std::string &)> &load, bool lazy, bool direct)
{
    if (typeid(*(args.front())) == typeid(CAS::NumberArith) && typeid(*(args[1])) == typeid(CAS::NumberArith) && typeid(*(args.back())) == typeid(CAS::NumberArith)) {
        CAS::AbstractExpression::Operands result;
        for (double i = std::move(static_cast<CAS::NumberArith*>(args.front().get())->getValue()); i <= static_cast<CAS::NumberArith*>(args[1].get())->getValue(); i += static_cast<CAS::NumberArith*>(args.back().get())->getValue())
            result.emplace_back(make_unique<CAS::NumberArith>(std::move(i)));
        return CAS::List(std::move(result)).eval(scope, load, lazy, direct).second;
    } else return nullptr;
}

CAS::AbstractExpression::ExpressionP sum(const CAS::AbstractExpression::Operands &args, CAS::Scope &scope, const std::function<void(const std::string &)> &load, bool lazy, bool direct)
{
    return static_cast<CAS::List*>(args.front().get())->sum()->eval(scope, load, lazy, direct).second;
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

CAS::AbstractExpression::ExpressionP binom(const CAS::AbstractExpression::Operands &args, CAS::Scope &, const std::function<void(const std::string &)> &, bool, bool)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberArith) || typeid(*(args.back())) != typeid(CAS::NumberArith)) return nullptr;
    return make_unique<CAS::NumberArith>(doBinom(static_cast<CAS::NumberArith*>(args.front().get())->getValue(), static_cast<CAS::NumberArith*>(args.back().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP binompdf(const CAS::AbstractExpression::Operands &args, CAS::Scope &, const std::function<void(const std::string &)> &, bool, bool)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberArith) || typeid(*(args.at(1))) != typeid(CAS::NumberArith) || typeid(*(args.back())) != typeid(CAS::NumberArith)) return nullptr;
    return make_unique<CAS::NumberArith>(doBinomPDF(static_cast<CAS::NumberArith*>(args.front().get())->getValue(), static_cast<CAS::NumberArith*>(args.at(1).get())->getValue(), static_cast<CAS::NumberArith*>(args.back().get())->getValue()));
}

CAS::AbstractExpression::ExpressionP binomcdf(const CAS::AbstractExpression::Operands &args, CAS::Scope &, const std::function<void(const std::string &)> &, bool, bool)
{
    if (typeid(*(args.front())) != typeid(CAS::NumberArith) || typeid(*(args.at(1))) != typeid(CAS::NumberArith) || typeid(*(args.back())) != typeid(CAS::NumberArith)) return nullptr;
    double result = 0;
    unsigned int start = static_cast<CAS::NumberArith*>(args.back().get())->getValue();
    do {
        result += doBinomPDF(static_cast<CAS::NumberArith*>(args.front().get())->getValue(), static_cast<CAS::NumberArith*>(args.at(1).get())->getValue(), start);
    } while (start--);
    return make_unique<CAS::NumberArith>(result);
}

}
