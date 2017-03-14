#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T16:21:21
#
#-------------------------------------------------

QT       += core gui sql network serialport
CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = aip-tester
TEMPLATE = app
win32{
LIBS += C:\CanCmd.dll
}
linux{
greaterThan(QT_MAJOR_VERSION, 4):LIBS += /usr/lib/libCanCmd.so
}

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
    PageOut.cpp \
    WinBack.cpp \
    TcpClient.cpp \
    UdpClient.cpp \
    SqlClient.cpp \
    CanClient.cpp \
    SerialPort.cpp

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
    PageOut.h \
    WinBack.h \
    TcpClient.h \
    UdpClient.h \
    SqlClient.h \
    CanClient.h \
    SerialPort.h

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
    PageLck.ui \
    PageOut.ui \
    WinBack.ui

RESOURCES += \
    image.qrc
