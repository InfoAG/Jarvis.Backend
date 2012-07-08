#include "DemoModules.h"

/*NumberExpression::state NumberExpression::delta(state stt, char str) const {
    switch (stt) {
    case START:
        if (str <= 57 && str >=49) return NFIRST;
        else if (str == '0') return ZFIRST;
        else if (str == '-' || str == '+') return VORZ;
        else return DEAD;
    case ACCPT:
        if (str <=57 && str >= 48) return ACCPT;
        else return DEAD;
    case ZFIRST:
        if (str == '.') return FIRSTT;
        else return DEAD;
    case NFIRST:
        if (str == '.') return FIRSTT;
        else if (str <=57 && str >= 48) return NFIRST;
        else return DEAD;
    case FIRSTT:
        if (str <=57 && str >= 48) return ACCPT;
        else return DEAD;
    case VORZ:
        if (str <= 57 && str >=49) return NFIRST;
        else return DEAD;
    }
}

std::unique_ptr<CAS::AbstractArithmetic> NumberExpression::parse(std::string candidate) const
{
    state stt = START;
    for (const auto &i : candidate)
        if ((stt = delta(stt, i)) == DEAD) return false;
    if (! (stt == ACCPT || stt == NFIRST || stt == ZFIRST)) return nullptr;
    else {
        return std::unique_ptr<CAS::NumberArith>(new CAS::NumberArith(strtod(candidate.c_str(), NULL)));
    }
}*/
