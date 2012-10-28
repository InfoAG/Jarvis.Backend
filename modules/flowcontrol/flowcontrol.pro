#-------------------------------------------------
#
# Project created by QtCreator 2012-09-23T19:11:13
#
#-------------------------------------------------

include(../../common.pri)

TARGET = flowcontrol

DEFINES += FLOWCONTROL_LIBRARY

SOURCES += flowcontrol.cpp \
    ScopeExpression.cpp \
    OutputExpression.cpp \
    BoolValue.cpp \
    IfExpression.cpp \
    EmptyExpression.cpp \
    EqualityExpression.cpp \
    NegationExpression.cpp \
    LogicalAndExpression.cpp \
    LogicalOrExpression.cpp \
    LessExpression.cpp \
    LessOrEqualExpression.cpp \
    GreaterExpression.cpp \
    GreaterOrEqualExpression.cpp \
    RangedForExpression.cpp

HEADERS +=\
        flowcontrol_global.h \
    ScopeExpression.h \
    OutputExpression.h \
    BoolValue.h \
    IfExpression.h \
    EmptyExpression.h \
    EqualityExpression.h \
    NegationExpression.h \
    LogicalAndExpression.h \
    LogicalOrExpression.h \
    LessExpression.h \
    LessOrEqualExpression.h \
    GreaterExpression.h \
    GreaterOrEqualExpression.h \
    RangedForExpression.h
