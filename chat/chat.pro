#-------------------------------------------------
#
# Project created by QtCreator 2019-11-20T10:18:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat
TEMPLATE = app

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

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    clientdialog.cpp \
    login.cpp \
    instruction.cpp \
    error.cpp \
    regerror.cpp \
    room.cpp \
    error_no.cpp \
    error_nn.cpp \
    registerin.cpp \
    const.cpp

HEADERS += \
        mainwindow.h \
    clientdialog.h \
    login.h \
    instruction.h \
    registerin.h \
    error.h \
    regerror.h \
    room.h \
    error_no.h \
    error_nn.h \
    const.h

FORMS += \
        mainwindow.ui \
    clientdialog.ui \
    login.ui \
    instruction.ui \
    error.ui \
    regerror.ui \
    room.ui \
    error_no.ui \
    error_nn.ui \
    registerin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
