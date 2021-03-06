#-------------------------------------------------
#
# Project created by QtCreator 2020-04-08T08:17:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mumblemove
TEMPLATE = app

VERSION = 0.1.0
DEFINES += VERSION=\\\"$${VERSION}\\\"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(../protocol/protocol.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mumblelink.cpp \
    avatar.cpp \
    scene.cpp \
    settingsdialog.cpp \
    mainview.cpp \
    client.cpp \
    scaleindicator.cpp

HEADERS += \
        mainwindow.h \
    mumblelink.h \
    avatar.h \
    scene.h \
    settingsdialog.h \
    mainview.h \
    client.h \
    scaleindicator.h

FORMS += \
        mainwindow.ui \
    settingsdialog.ui

unix {
    LIBS += -lrt
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
