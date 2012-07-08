#ifndef OPERATORINTERFACE_H
#define OPERATORINTERFACE_H

struct OperatorInterface
{
    enum AssociativityType { LEFT, RIGHT };

    bool (*matches)(std::string);
    int (*priority)();
    AssociativityType (*associativity)();
    std::unique_ptr<CAS::AbstractArithmetic> (*parse)(std::unique_ptr<CAS::AbstractArithmetic>, std::unique_ptr<CAS::AbstractArithmetic>);
};

#endif // OPERATORINTERFACE_H
