#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <algorithm>
#include <QDir>

#include "ParserModules.h"

class ExpressionParser
{
private:
    ParserModules modules;

public:
    ExpressionParser(const ParserModules &modules) : modules(modules) {};
    //ExpressionParser(ParserModules &&modules) : modules(std::forward<ParserModules>(modules)) {};

    void loadModules(const QDir &directory);
    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
