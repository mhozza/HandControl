#-------------------------------------------------
#
# Project created by QtCreator 2010-11-14T16:45:18
#
#-------------------------------------------------

QT       += core gui

TARGET = HandControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    webcam.cpp \
    merror.cpp \
    imageconvert.cpp \
    exceptions.cpp \
    imageprocessor.cpp

HEADERS  += mainwindow.h \
    webcam.h \
    imageconvert.h \
    merror.h \
    exceptions.h \
    imageprocessor.h

FORMS += mainwindow.ui

LIBS += -lv4l2
