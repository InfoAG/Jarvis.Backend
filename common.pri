INCLUDEPATH += ../..

TEMPLATE = lib

QT       -= gui

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../../Framework

CONFIG(debug, debug|release) {
    DESTDIR = ../../debug/modules/
    LIBS += -L../../../Framework/debug -lJarvis-Framework
    LIBS += -L../../debug -lJarvis-Backend
} else {
    DESTDIR = ../../release/modules/
    LIBS += -L../../../Framework/release -lJarvis-Framework
    LIBS += -L../../release -lJarvis-Backend
}
