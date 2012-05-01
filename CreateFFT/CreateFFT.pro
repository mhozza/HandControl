#-------------------------------------------------
#
# Project created by QtCreator 2012-04-20T15:38:47
#
#-------------------------------------------------

QT       += core gui

TARGET = RestoreFFT
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../restore_fft.cpp \
    ../utils.cpp \
    ../grayscaleimage.cpp

LIBS += -lfftw3
