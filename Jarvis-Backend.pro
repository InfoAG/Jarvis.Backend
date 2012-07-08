QT       += core network
QT       -= gui

TARGET = Jarvis-Backend
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#include(../../qt-solutions/qtservice/src/qtservice.pri)

QMAKE_CXXFLAGS += -std=c++11

#LIBS += ../Framework/debug/libJarvis-Framework.a

SOURCES += main.cpp \
    JarvisService.cpp \
    JarvisServer.cpp \
    ExpressionParser.cpp \
    DemoModules.cpp \
    ClientConnection.cpp \
    ModulePackage.cpp

HEADERS += \
    JarvisService.h \
    JarvisServer.h \
    ExpressionParser.h \
    DemoModules.h \
    FunctionModule.h \
    TerminalModule.h \
    OperatorModule.h \
    OperatorInterface.h \
    FunctionInterface.h \
    ModulePackage.h \
    ParserModules.h \
    ParserModule.h \
    ClientConnection.h
