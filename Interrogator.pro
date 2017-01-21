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
    wordprocessor.cpp

HEADERS  += mainwindow.h \
    wordprocessor.h

FORMS    += mainwindow.ui

CONFIG += c++11
