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
    System.cpp \
    MotorType.cpp \
    SqlPage.cpp \
    Histogram.cpp \
    Test.cpp \
    Waveform.cpp \
    Resistance.cpp \
    InputNum.cpp \
    PageKey.cpp \
    WinHome.cpp

HEADERS  += \
    System.h \
    MotorType.h \
    SqlPage.h \
    Histogram.h \
    Test.h \
    Waveform.h \
    Resistance.h \
    InputNum.h \
    PageKey.h \
    WinHome.h

FORMS    += \
    System.ui \
    MotorType.ui \
    SqlPage.ui \
    Test.ui \
    Resistance.ui \
    InputNum.ui \
    PageKey.ui \
    WinHome.ui

RESOURCES += \
    image.qrc \
    ../aip-local/aip-image.qrc
