#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T20:16:19
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = SoundRecorder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        leveloutput.cpp \
        levelbacking.cpp \
        soundengine.cpp \
        settingsdialog.cpp \
        levelrec.cpp \
        wavfile.cpp \
        utilities.cpp \
        soundtrack.cpp \
    spectrecontainer.cpp \
    audiospectre.cpp

HEADERS  += mainwindow.h \
        leveloutput.h \
        levelbacking.h \
        soundengine.h \
        settingsdialog.h \
        levelrec.h \
        wavfile.h \
        utilities.h \
        soundtrack.h \
    spectrecontainer.h \
    audiospectre.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc
