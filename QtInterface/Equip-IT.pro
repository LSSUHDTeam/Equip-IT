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
    framework/contextmanger.cpp

HEADERS  += windows/mainwindow.h \
    windows/basewindow.h \
    framework/closeeventfilter.h \
    framework/windowdescriptors.h \
    framework/sessionobject.h \
    framework/dataaccessmanager.h \
    framework/contextmanger.h \
    framework/structures.h

FORMS    += windows/mainwindow.ui \
    windows/basewindow.ui

DISTFILES += \
    Documentation/Data_Layout.md
