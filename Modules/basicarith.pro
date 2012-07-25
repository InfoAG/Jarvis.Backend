#-------------------------------------------------
#
# Project created by QtCreator 2012-07-09T14:16:58
#
#-------------------------------------------------

QT       -= gui #!!! remove core too
QMAKE_CXXFLAGS += -std=c++11
LIBS += ../../../../QtProjects/Jarvis/Framework/Jarvis-Framework/debug/libJarvis-Framework.a
TARGET = basicarith
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11
DEFINES += BASICARITH_LIBRARY

SOURCES += \
    basicarith.cpp

HEADERS +=\
        basicarith_global.h

