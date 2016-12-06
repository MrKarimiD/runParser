#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T00:59:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = runParser_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileparser.cpp

HEADERS  += mainwindow.h \
    fileparser.h

FORMS    += mainwindow.ui
