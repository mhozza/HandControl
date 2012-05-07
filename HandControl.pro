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
    NeuralNet/distributedneurallayer.cpp \
    NeuralNet/distributedneuralnetwork.cpp \
    kalmanfilter.cpp \
    colorimage.cpp \
    grayscaleimage.cpp \
    color.cpp \
    NeuralNet/elmannetwork.cpp \
    NeuralNet/recurrentlayer.cpp \
    NeuralNet/recurrentperceptron.cpp \
    NeuralNet/recurrentnetwork.cpp \
    NeuralNet/distributedrecurrentnetwork.cpp \
    NeuralNet/distributedrecurrentlayer.cpp

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
    NeuralNet/distributedneuralnetwork.h \
    kalmanfilter.h \
    colorimage.h \
    grayscaleimage.h \
    color.h \
    NeuralNet/elmannetwork.h \
    NeuralNet/recurrentlayer.h \
    NeuralNet/recurrentperceptron.h \
    NeuralNet/recurrentnetwork.h \
    NeuralNet/distributedrecurrentnetwork.h \
    NeuralNet/distributedrecurrentlayer.h

FORMS += mainwindow.ui \
    onscreendisplayform.ui

LIBS += -lv4l2 -lXtst -lfftw3 -lX11

OTHER_FILES +=
