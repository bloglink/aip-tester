#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T16:35:58
#
#-------------------------------------------------

QT       += core gui sql network serialport
CONFIG   += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tester
TEMPLATE = app
win32{
LIBS += C:\CanCmd.dll
}

SOURCES += main.cpp\
        desktop.cpp \
    sockeckcan.cpp \
    warnning.cpp \
    PageAcw.cpp \
    PageDcr.cpp \
    PageImp.cpp \
    PageInd.cpp \
    PageInr.cpp \
    PageKey.cpp \
    PageLck.cpp \
    PageLvs.cpp \
    PageMag.cpp \
    PageNum.cpp \
    PageOut.cpp \
    PagePwr.cpp \
    WinBack.cpp \
    WinData.cpp \
    WinSyst.cpp \
    WinTest.cpp \
    WinType.cpp \
    Histogram.cpp \
    SerialPort.cpp \
    SqlClient.cpp \
    TcpClient.cpp \
    UdpClient.cpp \
    WaveBox.cpp \
    Waveform.cpp \
    PageAmp.cpp

HEADERS  += desktop.h \
    sockeckcan.h \
    warnning.h \
    PageAcw.h \
    PageDcr.h \
    PageImp.h \
    PageInd.h \
    PageInr.h \
    PageKey.h \
    PageLck.h \
    PageLvs.h \
    PageMag.h \
    PageNum.h \
    PageOut.h \
    PagePwr.h \
    WinBack.h \
    WinData.h \
    WinSyst.h \
    WinTest.h \
    WinType.h \
    Histogram.h \
    ICANCmd.h \
    SerialPort.h \
    SqlClient.h \
    TcpClient.h \
    UdpClient.h \
    WaveBox.h \
    Waveform.h \
    PageAmp.h

FORMS    += desktop.ui \
    PageAcw.ui \
    PageDcr.ui \
    PageImp.ui \
    PageInd.ui \
    PageInr.ui \
    PageKey.ui \
    PageLck.ui \
    PageLvs.ui \
    PageMag.ui \
    PageNum.ui \
    PageOut.ui \
    PagePwr.ui \
    WinBack.ui \
    WinData.ui \
    WinHome.ui \
    WinSyst.ui \
    WinTest.ui \
    WinType.ui

RESOURCES += \
    image.qrc
