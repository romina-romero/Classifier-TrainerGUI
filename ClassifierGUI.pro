#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T20:09:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClassifierGUI
TEMPLATE = app

SOURCES +=  main.cpp\
            mainwindow.cpp \
            trainer.cpp \
    configurationwindow.cpp \
    classifier.cpp \
    detector.cpp \
    test.cpp

HEADERS  += mainwindow.h \
            trainer.h \
    configurationwindow.h \
    classifier.h \
    detector.h \
    test.h

FORMS    += mainwindow.ui \
    configurationwindow.ui

LIBS += -L/usr/local/lib -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_objdetect
LIBS += -lopencv_features2d
LIBS += -lopencv_ml
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_highgui

