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
        HomePage.cpp \
    System.cpp \
    Input.cpp \
    MotorType.cpp \
    SqlPage.cpp \
    Histogram.cpp

HEADERS  += HomePage.h \
    System.h \
    Input.h \
    MotorType.h \
    SqlPage.h \
    Histogram.h

FORMS    += HomePage.ui \
    System.ui \
    Input.ui \
    MotorType.ui \
    SqlPage.ui

RESOURCES += \
    image.qrc \
    ../aip-local/aip-image.qrc
