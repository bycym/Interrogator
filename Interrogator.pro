#-------------------------------------------------
#
# Project created by QtCreator 2016-11-27T15:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Interrogator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wordprocessor.cpp \
    achivementdialog.cpp

HEADERS  += mainwindow.h \
    wordprocessor.h \
    achivementdialog.h

FORMS    += mainwindow.ui \
    achivementdialog.ui

CONFIG += c++11

DISTFILES += \
    Images/100.png \
    Images/500.png \
    Images/1000.png
