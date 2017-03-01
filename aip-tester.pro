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
    Waveform.cpp \
    InputNum.cpp \
    PageKey.cpp \
    WinHome.cpp \
    WinSyst.cpp \
    WinType.cpp \
    WinData.cpp \
    WinTest.cpp \
    PageDcr.cpp \
    PageMag.cpp \
    PageInr.cpp \
    PageAcw.cpp

HEADERS  += \
    Histogram.h \
    Waveform.h \
    InputNum.h \
    PageKey.h \
    WinHome.h \
    WinSyst.h \
    WinType.h \
    WinData.h \
    WinTest.h \
    PageDcr.h \
    PageMag.h \
    PageInr.h \
    PageAcw.h

FORMS    += \
    InputNum.ui \
    PageKey.ui \
    WinHome.ui \
    WinSyst.ui \
    WinType.ui \
    WinData.ui \
    WinTest.ui \
    PageDcr.ui \
    PageMag.ui \
    PageInr.ui \
    PageAcw.ui

RESOURCES += \
    image.qrc \
    ../aip-local/aip-image.qrc
