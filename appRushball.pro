#-------------------------------------------------
#
# Project created by QtCreator 2020-05-14T14:19:33
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = appRushball
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
    cjeu.cpp \
    cbdd.cpp \
    ccommcibles.cpp \
    ccommpupitre.cpp \
    ccommaffichage.cpp \
    czdc.cpp \
    ../biblis/ci2c.cpp \
    cserveurtcp.cpp \
    cprotocoleclient.cpp \
    cgererclient.cpp

HEADERS += \
        cihm.h \
    cjeu.h \
    cbdd.h \
    ccommcibles.h \
    ccommpupitre.h \
    ccommaffichage.h \
    czdc.h \
    ../biblis/ci2c.h \
    communs.h \
    cserveurtcp.h \
    cprotocoleclient.h \
    cgererclient.h

FORMS += \
        cihm.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
