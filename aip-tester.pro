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
    PageKey.cpp \
    WinHome.cpp \
    WinSyst.cpp \
    WinType.cpp \
    WinData.cpp \
    WinTest.cpp \
    PageDcr.cpp \
    PageMag.cpp \
    PageInr.cpp \
    PageAcw.cpp \
    PageImp.cpp \
    PageNum.cpp \
    PageInd.cpp \
    PagePwr.cpp \
    PageLvs.cpp \
    PageLck.cpp \
    CanSocket.cpp \
    SqlData.cpp

HEADERS  += \
    Histogram.h \
    Waveform.h \
    PageKey.h \
    WinHome.h \
    WinSyst.h \
    WinType.h \
    WinData.h \
    WinTest.h \
    PageDcr.h \
    PageMag.h \
    PageInr.h \
    PageAcw.h \
    PageImp.h \
    PageNum.h \
    PageInd.h \
    PagePwr.h \
    PageLvs.h \
    PageLck.h \
    CanSocket.h \
    SqlData.h

FORMS    += \
    PageKey.ui \
    WinHome.ui \
    WinSyst.ui \
    WinType.ui \
    WinData.ui \
    WinTest.ui \
    PageDcr.ui \
    PageMag.ui \
    PageInr.ui \
    PageAcw.ui \
    PageImp.ui \
    PageNum.ui \
    PageInd.ui \
    PagePwr.ui \
    PageLvs.ui \
    PageLck.ui

RESOURCES += \
    image.qrc \
    ../aip-local/aip-image.qrc
