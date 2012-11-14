#include "std_global.h"
#include "expression/AbstractExpression.h"
#include "expression/NumberArith.h"
#include "expression/Function.h"
#include "expression/List.h"

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
        for (CAS::Integer i = std::move(static_cast<CAS::NumberArith*>(args.front().get())->getValue()); i <= static_cast<CAS::NumberArith*>(args[1].get())->getValue(); i += static_cast<CAS::NumberArith*>(args.back().get())->getValue())
            result.emplace_back(make_unique<CAS::NumberArith>(std::move(i)));
        return CAS::List(std::move(result)).eval(scope, load, lazy, direct).second;
    } else return nullptr;
}

}
