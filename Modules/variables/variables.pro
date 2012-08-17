#-------------------------------------------------
#
# Project created by QtCreator 2012-08-15T17:26:13
#
#-------------------------------------------------

QT       -= gui

TARGET = variables
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/../../../Framework
INCLUDEPATH += $$PWD/../../

DEFINES += VARIABLES_LIBRARY

SOURCES += Variables.cpp

HEADERS +=\
        variables_global.h

Release {
    DESTDIR = $$PWD/../../release/Modules
    LIBS += -L$$PWD/../../../Framework/release -lJarvis-Framework
}
Debug {
    DESTDIR = $$PWD/../../debug/Modules
    LIBS += -L$$PWD/../../../Framework/debug -lJarvis-Framework
}
