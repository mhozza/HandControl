#-------------------------------------------------
#
# Project created by QtCreator 2011-03-24T17:36:52
#
#-------------------------------------------------
TARGET = NeuralNet
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    perceptron.cpp \
    continuous_perceptron.cpp \
    neuralnetwork.cpp \
    neurallayer.cpp \
    distributedneurallayer.cpp \
    distributedneuralnetwork.cpp

HEADERS += \
    perceptron.h \
    continuous_perceptron.h \
    neuralnetwork.h \
    neurallayer.h \
    distributedneurallayer.h \
    distributedneuralnetwork.h

QMAKE_CXXFLAGS_RELEASE += -O3

OTHER_FILES +=
