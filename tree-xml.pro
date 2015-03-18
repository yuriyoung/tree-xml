#-------------------------------------------------
#
# Project created by QtCreator 2013-07-01T09:55:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tree-xml
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treewidget.cpp \
    xmlreader.cpp \
    xmlwriter.cpp

HEADERS  += mainwindow.h \
    treewidget.h \
    xmlreader.h \
    xmlwriter.h \
    constance.h

FORMS    += mainwindow.ui
