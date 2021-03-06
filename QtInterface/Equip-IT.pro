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
    dialogs/timegetter.cpp \
    dialogs/quicktimeget.cpp \
    windows/buildingselection.cpp \
    dialogs/addbuilding.cpp \
    dialogs/reservationviewer.cpp \
    dialogs/finalizereservation.cpp \
    dialogs/simplemessagebox.cpp \
    windows/errorresolver.cpp \
    windows/reservationroot.cpp \
    windows/catitems.cpp \
    dialogs/iteminfodisplay.cpp \
    dialogs/itemconfirmationdialog.cpp \
    dialogs/manualtimeentry.cpp \
    windows/configurerepetition.cpp \
    windows/bettertimeget.cpp \
    dialogs/bettertimeselection.cpp

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
    dialogs/timegetter.h \
    dialogs/quicktimeget.h \
    windows/buildingselection.h \
    dialogs/addbuilding.h \
    dialogs/reservationviewer.h \
    dialogs/finalizereservation.h \
    dialogs/simplemessagebox.h \
    windows/errorresolver.h \
    windows/reservationroot.h \
    windows/catitems.h \
    dialogs/iteminfodisplay.h \
    dialogs/itemconfirmationdialog.h \
    dialogs/manualtimeentry.h \
    windows/configurerepetition.h \
    windows/bettertimeget.h \
    dialogs/bettertimeselection.h

FORMS    += windows/mainwindow.ui \
    windows/basewindow.ui \
    windows/displaynetworkerror.ui \
    windows/adminpanel.ui \
    windows/checkout.ui \
    windows/screenboard.ui \
    windows/quickscanitem.ui \
    windows/peripheralvalidator.ui \
    dialogs/timegetter.ui \
    dialogs/quicktimeget.ui \
    windows/buildingselection.ui \
    dialogs/addbuilding.ui \
    dialogs/reservationviewer.ui \
    dialogs/finalizereservation.ui \
    dialogs/simplemessagebox.ui \
    windows/errorresolver.ui \
    windows/reservationroot.ui \
    windows/catitems.ui \
    dialogs/iteminfodisplay.ui \
    dialogs/itemconfirmationdialog.ui \
    dialogs/manualtimeentry.ui \
    windows/configurerepetition.ui \
    windows/bettertimeget.ui \
    dialogs/bettertimeselection.ui

DISTFILES += \
    Documentation/Data_Layout.md

RESOURCES +=
