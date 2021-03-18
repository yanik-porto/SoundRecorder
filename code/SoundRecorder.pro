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
        settingsdialog.cpp \
    src/levels/level.cpp \
    src/levels/level_horizontal.cpp \
    src/levels/level_vertical.cpp \
    src/sound_engine.cpp \
    src/sound_recorder_exception.cpp \
    src/sound_track.cpp \
        wavfile.cpp \
        utilities.cpp \
    spectrecontainer.cpp \
    audiospectre.cpp

HEADERS  += mainwindow.h \
    include/private/levels/level.h \
    include/private/levels/level_horizontal.h \
    include/private/levels/level_vertical.h \
    include/private/sound_engine.h \
    include/private/sound_track.h \
    include/public/i_sound_engine.h \
    include/public/i_sound_track.h \
    include/public/levels/i_level.h \
    include/public/sound_recorder_exception.h \
        settingsdialog.h \
        wavfile.h \
        utilities.h \
    spectrecontainer.h \
    audiospectre.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc
