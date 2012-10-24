#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QFile>
#include <string>
#include <map>
#include <QLibrary>
#include "ParserModules.h"
#include <QFileInfo>
#include <QTextStream>

/**
 * Package of parser modules for an ExpressionParser
 */
class ModulePackage
{
public:
    ModulePackage() {} //!< Default constructor
    /**
     * Contruct a module package by reading modules from a text file and loading symbols from the referenced libs
     * @param file File to load from
     */
    ModulePackage(std::unique_ptr<QFile> file);

    const ParserModules &getModules() const { return modules; } //!< @return Parser modules in this package
    QString name() const { return name_; } //!< @return Package name

private:
    QString name_; //!< Package name
    ParserModules modules; //!< Modules in this package
};

/**
 * Writes a module package to the stream
 * @param stream Reference to the stream
 * @param pkg Package to write
 * @return Reference to the stream
 */
QDataStream &operator<<(QDataStream &stream, const ModulePackage &pkg);

#endif // MODULEPACKAGE_H
