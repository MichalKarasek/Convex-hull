#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T09:25:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chull
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    algorithms.cpp \
    sortbyyasc.cpp \
    sortbyxasc.cpp \
    sets.cpp \
    graphic.cpp

HEADERS += \
        widget.h \
    algorithms.h \
    sortbyyasc.h \
    sortbyxasc.h \
    sets.h \
    graphic.h

FORMS += \
        widget.ui
