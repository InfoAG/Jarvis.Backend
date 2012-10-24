#-------------------------------------------------
#
# Project created by QtCreator 2012-08-19T12:33:27
#
#-------------------------------------------------

QT       += core network

QT       -= gui

release {
    DESTDIR = ../release/
    LIBS += -L../../Framework/release -lJarvis-Framework
}
debug {
    DESTDIR = ../release/
    LIBS += -L../../Framework/debug -lJarvis-Framework
}

TARGET = Jarvis-Backend

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += .. \
    ../../Framework

VPATH += ../

SOURCES += \
    JarvisService.cpp \
    JarvisServer.cpp \
    ExpressionParser.cpp \
    ClientConnection.cpp \
    ModulePackage.cpp \
    Room.cpp \
    ParserModules.cpp \
    ParserModule.cpp \
    FunctionModule.cpp \
    RoomScope.cpp \
    ../BinaryOperatorModule.cpp \
    ../UnaryOperatorModule.cpp \
    ../TerminalModule.cpp

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
    Room.h \
    RoomScope.h \
    ../BinaryOperatorModule.h \
    ../BinaryOperatorInterface.h \
    ../UnaryOperatorInterface.h \
    ../UnaryOperatorModule.h \
    ../BinaryOperatorStatics.h \
    ../UnaryOperatorStatics.h \
    ../OperatorStatics.h

OTHER_FILES +=  OperatorModule.cpp
