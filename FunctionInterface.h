#ifndef FUNCTIONINTERFACE_H
#define FUNCTIONINTERFACE_H

struct FunctionInterface
{
    bool (*matches)(std::string);
    std::unique_ptr<CAS::AbstractArithmetic> (*parse)(std::unique_ptr<CAS::AbstractArithmetic>);
};

#endif // FUNCTIONINTERFACE_H
