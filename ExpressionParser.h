#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../Framework/Container/SmartList.h"
#include <QDir>

#include "ModulePackage.h"
#include "ParserModules.h"

class ExpressionParser
{
private:
    ParserModules modules;
    CAS::SmartList<ModulePackage> module_pkgs;

public:
    ExpressionParser(const QDir &module_dir);
    //ExpressionParser(ParserModules &&modules) : modules(std::forward<ParserModules>(modules)) {};

    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
