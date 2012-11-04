#include "FunctionDefinitionExpression.h"

CAS::AbstractExpression::EvalRes FunctionDefinitionExpression::eval(CAS::Scope &scope, bool lazy) const
{
    if (head.getArgNames().empty() && ! head.getSignature().argumentTypes.empty()) throw "gimme names bro";
    if (! scope.hasFunc(head.getSignature())) head.eval(scope, lazy);
    scope.defineFunc(head.getSignature(), {body->copy(), head.getArgNames(), head.getReturnType()});
    return std::make_pair(CAS::TypeInfo::VOID, copy());
}
