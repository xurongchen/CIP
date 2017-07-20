#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T09:37:37
#
#-------------------------------------------------

QT       += core gui\
           sql

TARGET = Insurance
TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    adminoper.cpp \
    test.cpp \
    dboperation.cpp \
    currentuser.cpp \
    selleroper.cpp \
    information.cpp \
    manageroper.cpp \
    pop.cpp \
    insurance.cpp \
    policyinsurance.cpp \
    detail.cpp

HEADERS  += mainwindow.h \
    dbconnect.h \
    adminoper.h \
    test.h \
    dboperation.h \
    currentuser.h \
    selleroper.h \
    information.h \
    manageroper.h \
    pop.h \
    insurance.h \
    policyinsurance.h \
    detail.h

FORMS    += mainwindow.ui \
    adminoper.ui \
    selleroper.ui \
    manageroper.ui \
    pop.ui \
    detail.ui

RESOURCES += \
    new.qrc
