QT += core
QT -= gui
TEMPLATE = app
TARGET = d1_tests
CONFIG += console c++11
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../model $$PWD/../controller

SOURCES += $$PWD/d1_tests.cpp
SOURCES += $$files($$PWD/../model/*.cpp)
SOURCES += $$files($$PWD/../controller/*.cpp)

HEADERS += $$files($$PWD/../model/*.h)
HEADERS += $$files($$PWD/../controller/*.h)
