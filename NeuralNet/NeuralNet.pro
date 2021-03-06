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
    distributedneuralnetwork.cpp \
    elmannetwork.cpp \
    recurrentlayer.cpp \
    recurrentnetwork.cpp \
    recurrentperceptron.cpp \
    distributedrecurrentnetwork.cpp \
    distributedrecurrentlayer.cpp

HEADERS += \
    perceptron.h \
    continuous_perceptron.h \
    neuralnetwork.h \
    neurallayer.h \
    distributedneurallayer.h \
    distributedneuralnetwork.h \
    elmannetwork.h \
    recurrentlayer.h \
    recurrentnetwork.h \
    recurrentperceptron.h \
    distributedrecurrentnetwork.h \
    distributedrecurrentlayer.h

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

OTHER_FILES +=
