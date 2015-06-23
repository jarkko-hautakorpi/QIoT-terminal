#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T19:12:34
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QIoT-terminal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    lookuptables.h

FORMS    += mainwindow.ui
