#-------------------------------------------------
#
# Project created by QtCreator 2017-10-14T10:37:25
#
#-------------------------------------------------

QT       += core gui sql xml network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = aip-tester
TEMPLATE = app

HEADERS += \
    app/action.h \
    app/author.h \
    app/backup.h \
    app/config.h \
    app/global.h \
    app/impulse.h \
    app/insulation.h \
    app/logger.h \
    app/lqcombobox.h \
    app/lqdoublespinbox.h \
    app/lqmessagebox.h \
    app/lqreadonlyitem.h \
    app/lqsnowflake.h \
    app/lqspinbox.h \
    app/lqsqltablemodel.h \
    app/lqstandarditem.h \
    app/magnetic.h \
    app/mainscreen.h \
    app/master.h \
    app/resistance.h \
    app/signin.h \
    app/source.h \
    app/sqlite.h \
    app/system.h \
    app/tester.h \
    app/udpsocket.h \
    app/voltage.h \
    app/wt330.h \
    lib/qcustomplot.h \
    app/iobrd.h

SOURCES += \
    app/action.cpp \
    app/author.cpp \
    app/backup.cpp \
    app/config.cpp \
    app/global.cpp \
    app/impulse.cpp \
    app/insulation.cpp \
    app/logger.cpp \
    app/lqcombobox.cpp \
    app/lqdoublespinbox.cpp \
    app/lqmessagebox.cpp \
    app/lqreadonlyitem.cpp \
    app/lqsnowflake.cpp \
    app/lqspinbox.cpp \
    app/lqsqltablemodel.cpp \
    app/lqstandarditem.cpp \
    app/magnetic.cpp \
    app/main.cpp \
    app/mainscreen.cpp \
    app/master.cpp \
    app/resistance.cpp \
    app/signin.cpp \
    app/source.cpp \
    app/sqlite.cpp \
    app/system.cpp \
    app/tester.cpp \
    app/udpsocket.cpp \
    app/voltage.cpp \
    app/wt330.cpp \
    lib/qcustomplot.cpp \
    app/iobrd.cpp

RC_FILE += qrc/ico.rc


