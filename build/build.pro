#-------------------------------------------------
#
# Project created by QtCreator 2012-08-19T12:33:27
#
#-------------------------------------------------

QT       += core network

QT       -= gui

Debug: DESTDIR = ../debug/
Release: DESTDIR = ../release/

TARGET = Jarvis-Backend

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += .. \
    ../../Framework

VPATH += ../

SOURCES += main.cpp \
    JarvisService.cpp \
    JarvisServer.cpp \
    ExpressionParser.cpp \
    ClientConnection.cpp \
    ModulePackage.cpp \
    Scope.cpp \
    ParserModules.cpp \
    ParserModule.cpp \
    ../OperatorModule.cpp \
    ../FunctionModule.cpp

HEADERS += \
    JarvisService.h \
    JarvisServer.h \
    ExpressionParser.h \
    FunctionModule.h \
    TerminalModule.h \
    OperatorModule.h \
    OperatorInterface.h \
    FunctionInterface.h \
    ModulePackage.h \
    ParserModules.h \
    ParserModule.h \
    ClientConnection.h \
    Scope.h
