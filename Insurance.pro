#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T09:37:37
#
#-------------------------------------------------

QT       += core gui\
           sql

TARGET = Insurance
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dboperation.cpp \
    test.cpp

HEADERS  += mainwindow.h \
    dbconnect.h \
    dboperation.h \
    test.h

FORMS    += mainwindow.ui

RESOURCES += \
    new.qrc
