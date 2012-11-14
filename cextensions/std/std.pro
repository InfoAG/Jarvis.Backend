#-------------------------------------------------
#
# Project created by QtCreator 2012-11-09T17:35:51
#
#-------------------------------------------------

QT       -= core gui

TARGET = std
TEMPLATE = lib

DEFINES += STD_LIBRARY

SOURCES += std.cpp

HEADERS +=\
        std_global.h

INCLUDEPATH += ../..

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../../Framework

release {
    DESTDIR = ../../release/cextensions/
    LIBS += -L../../../Framework/release -lJarvis-Framework
    LIBS += -L../../release -lJarvis-Backend
}
debug {
    DESTDIR = ../../debug/cextensions/
    LIBS += -L../../../Framework/debug -lJarvis-Framework
    LIBS += -L../../debug -lJarvis-Backend
}
