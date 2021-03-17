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
        soundengine.cpp \
        settingsdialog.cpp \
    src/levels/level.cpp \
    src/levels/level_horizontal.cpp \
    src/levels/level_vertical.cpp \
        wavfile.cpp \
        utilities.cpp \
        soundtrack.cpp \
    spectrecontainer.cpp \
    audiospectre.cpp

HEADERS  += mainwindow.h \
    include/private/levels/level.h \
    include/private/levels/level_horizontal.h \
    include/private/levels/level_vertical.h \
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
