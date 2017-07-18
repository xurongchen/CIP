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
    adminoper.cpp \
    test.cpp \
    dboperation.cpp \
    currentuser.cpp \
    selleroper.cpp \
    information.cpp \
    manageroper.cpp

HEADERS  += mainwindow.h \
    dbconnect.h \
    adminoper.h \
    test.h \
    dboperation.h \
    currentuser.h \
    selleroper.h \
    information.h \
    manageroper.h

FORMS    += mainwindow.ui \
    adminoper.ui \
    selleroper.ui \
    manageroper.ui

RESOURCES += \
    new.qrc
