#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T16:21:21
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-tester
TEMPLATE = app


SOURCES += main.cpp\
    Histogram.cpp \
    Test.cpp \
    Waveform.cpp \
    Resistance.cpp \
    InputNum.cpp \
    PageKey.cpp \
    WinHome.cpp \
    WinSyst.cpp \
    WinType.cpp \
    WinData.cpp \
    WinTest.cpp

HEADERS  += \
    Histogram.h \
    Test.h \
    Waveform.h \
    Resistance.h \
    InputNum.h \
    PageKey.h \
    WinHome.h \
    WinSyst.h \
    WinType.h \
    WinData.h \
    WinTest.h

FORMS    += \
    Test.ui \
    Resistance.ui \
    InputNum.ui \
    PageKey.ui \
    WinHome.ui \
    WinSyst.ui \
    WinType.ui \
    WinData.ui \
    WinTest.ui

RESOURCES += \
    image.qrc \
    ../aip-local/aip-image.qrc