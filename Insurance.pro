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
    adminoper.cpp

HEADERS  += mainwindow.h \
    dbconnect.h \
    adminoper.h

FORMS    += mainwindow.ui \
    adminoper.ui

RESOURCES += \
    new.qrc
