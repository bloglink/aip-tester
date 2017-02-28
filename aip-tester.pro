#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T16:21:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aip-tester
TEMPLATE = app


SOURCES += main.cpp\
        HomePage.cpp \
    System.cpp \
    Input.cpp

HEADERS  += HomePage.h \
    System.h \
    Input.h

FORMS    += HomePage.ui \
    System.ui \
    Input.ui

RESOURCES += \
    image.qrc
