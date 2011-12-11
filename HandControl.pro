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
    NeuralNet/continuous_perceptron.cpp \
    gesturerecognizer.cpp \
    gesture.cpp \
    gesturemoveleft.cpp \
    gesturemoveright.cpp \
    xfunc.cpp \
    gesturemoveup.cpp \
    gesturemovedown.cpp \
    onscreendisplayform.cpp \
    utils.cpp \
    hcimage.cpp \
    NeuralNet/distributedneurallayer.cpp \
    NeuralNet/distributedneuralnetwork.cpp

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
    NeuralNet/continuous_perceptron.h \
    gesturerecognizer.h \
    gesture.h \
    gesturemoveleft.h \
    gesturemoveright.h \
    xfunc.h \
    gesturemoveup.h \
    gesturemovedown.h \
    onscreendisplayform.h \
    utils.h \
    hcimage.h \
    NeuralNet/distributedneurallayer.h \
    NeuralNet/distributedneuralnetwork.h

FORMS += mainwindow.ui \
    onscreendisplayform.ui

LIBS += -lv4l2 -lXtst -lfftw3

OTHER_FILES += \
    NeuralNet/blabla
