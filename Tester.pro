#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T16:35:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tester
TEMPLATE = app


SOURCES += main.cpp\
        desktop.cpp \
    sockeckcan.cpp

HEADERS  += desktop.h \
    sockeckcan.h

FORMS    += desktop.ui

RESOURCES += \
    image.qrc
