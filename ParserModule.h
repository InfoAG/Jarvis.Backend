#ifndef PARSERMODULE_H
#define PARSERMODULE_H

#include <QFile>
#include <QTextStream>
#include <QLibrary>

class ModulePackage;

/**
 * Parser
 */
class ParserModule
{
public:
    ParserModule(const QString &name, const ModulePackage *parent_pkg) : name_(name), parent_pkg(parent_pkg) {}
    ParserModule(const QString &name, const QString &description, const ModulePackage *parent_pkg) : name_(name), description_(description), parent_pkg(parent_pkg) {};
    ~ParserModule() { lib.unload(); }

    bool inPkg(ModulePackage *pkg) const { return parent_pkg == pkg; }
    QString name() const { return name_; }
    QString description() const { return description_; }

    ParserModule &operator=(const ParserModule &other) { name_ = other.name_; description_ = other.description_; parent_pkg = other.parent_pkg; return *this; }

protected:
    QString name_;
    QString description_;
    const ModulePackage *parent_pkg;
    QLibrary lib;

    std::pair<QString, QString> readProperty(QTextStream &stream);
};

QDataStream &operator<<(QDataStream &stream, const ParserModule &module);

#endif // PARSERMODULE_H
