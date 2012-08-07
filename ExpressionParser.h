#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../Framework/Arithmetic/AbstractArithmetic.h"
#include <QDir>
#include "ParserModules.h"
#include "ModulePackage.h"

class ExpressionParser
{
private:
    ParserModules modules;
    QList<std::shared_ptr<ModulePackage> > modulePkgs;

public:
    ExpressionParser(const QDir &module_dir);
    void unload(const QString &pkgName);
    std::shared_ptr<ModulePackage> load(const QString &pkgName);
    QVector<ModulePackage> getModulePkgs() const;

    std::unique_ptr<CAS::AbstractArithmetic> parse(std::string input);
};

#endif //PARSER_H
