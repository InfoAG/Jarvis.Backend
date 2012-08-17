#-------------------------------------------------
#
# Project created by QtCreator 2012-07-09T14:16:58
#
#-------------------------------------------------

QT       -= gui #!!! remove core too
QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += $$PWD/../../../Framework
INCLUDEPATH += $$PWD/../../

TARGET = basicarith
TEMPLATE = lib
DEFINES += BASICARITH_LIBRARY

Release {
    DESTDIR = $$PWD/../../release/Modules
    LIBS += -L$$PWD/../../../Framework/release -lJarvis-Framework
}
Debug {
    DESTDIR = $$PWD/../../debug/Modules
    LIBS += -L$$PWD/../../../Framework/debug -lJarvis-Framework
}

SOURCES += \
    basicarith.cpp

HEADERS +=\
        basicarith_global.h

