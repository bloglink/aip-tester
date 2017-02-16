#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T09:39:47
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-local
TEMPLATE = app
LIBS += /usr/lib/libCanCmd.so

SOURCES += main.cpp \
    CWinBack.cpp \
    CWinData.cpp \
    CWinDbug.cpp \
    CWinSetAcw.cpp \
    CWinSetDcr.cpp \
    CWinSetImp.cpp \
    CWinSetIr.cpp \
    CWinSetMag.cpp \
    CWinTest.cpp \
    Waveform.cpp \
    CWinType.cpp \
    HomePage.cpp \
    Settings.cpp \
    CanSocket.cpp \
    MainPage.cpp \
    InputNum.cpp \
    InputKey.cpp \
    ConfigBlock.cpp \
    ConfigIndl.cpp \
    ConfigLvs.cpp \
    DigitalBox.cpp \
    ConfigPower.cpp \
    SqlContrl.cpp

HEADERS  += \
    CWinBack.h \
    CWinData.h \
    CWinDbug.h \
    CWinSetAcw.h \
    CWinSetDcr.h \
    CWinSetImp.h \
    CWinSetIr.h \
    CWinSetMag.h \
    CWinTest.h \
    Waveform.h \
    CWinType.h \
    HomePage.h \
    Settings.h \
    CanSocket.h \
    MainPage.h \
    InputNum.h \
    main.h \
    InputKey.h \
    ConfigBlock.h \
    ConfigIndl.h \
    ConfigLvs.h \
    DigitalBox.h \
    ConfigPower.h \
    SqlContrl.h

FORMS    += \
    CWinBack.ui \
    CWinData.ui \
    CWinDbug.ui \
    CWinSetAcw.ui \
    CWinSetDcr.ui \
    CWinSetImp.ui \
    CWinSetIr.ui \
    CWinSetMag.ui \
    CWinTest.ui \
    CWinType.ui \
    HomePage.ui \
    Settings.ui \
    MainPage.ui \
    InputNum.ui \
    InputKey.ui \
    ConfigBlock.ui \
    ConfigIndl.ui \
    ConfigLvs.ui \
    ConfigPower.ui

RESOURCES += \
    aip-image.qrc
