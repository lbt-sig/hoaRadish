TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_CXXFLAGS_RELEASE += -O3

INCLUDEPATH += /usr/local/include/opencv \
               /usr/local/include \
              ../../include

LIBS +=/usr/local/lib/lib*


SOURCES += \
    ../../src/armor.cpp \
    ../../main.cpp

HEADERS += ../../include/armor/armor.h
