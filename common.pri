INCLUDEPATH += ../..

TEMPLATE = lib

QT       -= gui

TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../../Framework

Release {
    DESTDIR = ../../release/modules
    LIBS += -L../../../Framework/release -lJarvis-Framework
}
Debug {
    DESTDIR = ../../debug/modules
    LIBS += -L../../../Framework/debug -lJarvis-Framework
}
