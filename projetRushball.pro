#-------------------------------------------------
#
# Project created by QtCreator 2021-05-06T14:57:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport network

TARGET = projetRushball
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
        cihm.cpp \
    ccommaffichage.cpp \
    ccommpanneaux.cpp \
    cgererpupitre.cpp \
    ci2c.cpp \
    cjeu.cpp \
    cprotocolepanneaux.cpp \
    czdc.cpp \
    cgererclient.cpp \
    cserveurtcp.cpp \
    cprotocoleclient.cpp

HEADERS += \
        cihm.h \
    ccommaffichage.h \
    ccommpanneaux.h \
    cgererpupitre.h \
    ci2c.h \
    cjeu.h \
    communs.h \
    cprotocolepanneaux.h \
    czdc.h \
    cgererclient.h \
    cserveurtcp.h \
    cprotocoleclient.h

FORMS += \
        cihm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
