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
    imageprocessor.cpp \
    handrecognizer.cpp \
    NeuralNet/perceptron.cpp \
    NeuralNet/neuralnetwork.cpp \
    NeuralNet/neurallayer.cpp \
    NeuralNet/continuous_perceptron.cpp

HEADERS  += mainwindow.h \
    webcam.h \
    imageconvert.h \
    merror.h \
    exceptions.h \
    imageprocessor.h \
    handrecognizer.h \
    NeuralNet/perceptron.h \
    NeuralNet/neuralnetwork.h \
    NeuralNet/neurallayer.h \
    NeuralNet/continuous_perceptron.h

FORMS += mainwindow.ui

LIBS += -lv4l2
