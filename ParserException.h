#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include "exception/JarvisException.h"

class ParserException : public CAS::JarvisException
{
private:
    std::string expr;

public:
    ParserException(std::string expr) : expr(std::move(expr)) {}
    virtual std::string what() const { return "could not parse \"" + expr + "\""; }
};

#endif // PARSEREXCEPTION_H
