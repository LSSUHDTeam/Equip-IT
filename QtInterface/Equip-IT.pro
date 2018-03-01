#-------------------------------------------------
#
# Project created by QtCreator 2018-02-20T08:24:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Equip-IT
TEMPLATE = app


SOURCES += main.cpp\
    windows/mainwindow.cpp \
    windows/basewindow.cpp \
    framework/closeeventfilter.cpp \
    framework/sessionobject.cpp \
    framework/dataaccessmanager.cpp \
    framework/contextmanger.cpp \
    windows/displaynetworkerror.cpp \
    windows/adminpanel.cpp \
    windows/checkout.cpp \
    windows/screenboard.cpp \
    objects/etouchlineedit.cpp \
    framework/ephimeral.cpp \
    windows/quickscanitem.cpp \
    windows/peripheralvalidator.cpp \
    windows/reservationviewer.cpp

HEADERS  += windows/mainwindow.h \
    windows/basewindow.h \
    framework/closeeventfilter.h \
    framework/windowdescriptors.h \
    framework/sessionobject.h \
    framework/dataaccessmanager.h \
    framework/contextmanger.h \
    framework/structures.h \
    windows/displaynetworkerror.h \
    windows/adminpanel.h \
    windows/checkout.h \
    windows/screenboard.h \
    objects/etouchlineedit.h \
    framework/ephimeral.h \
    windows/quickscanitem.h \
    windows/peripheralvalidator.h \
    windows/reservationviewer.h

FORMS    += windows/mainwindow.ui \
    windows/basewindow.ui \
    windows/displaynetworkerror.ui \
    windows/adminpanel.ui \
    windows/checkout.ui \
    windows/screenboard.ui \
    windows/quickscanitem.ui \
    windows/peripheralvalidator.ui \
    windows/reservationviewer.ui

DISTFILES += \
    Documentation/Data_Layout.md

RESOURCES +=
