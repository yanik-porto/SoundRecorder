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
    src/entities/audio_spectre.cpp \
    src/entities/settings_dialog.cpp \
    src/entities/sound_engine.cpp \
    src/entities/sound_track.cpp \
    src/entities/spectre_container.cpp \
    src/factories/sound_tracks.cpp \
    src/levels/level.cpp \
    src/levels/level_horizontal.cpp \
    src/levels/level_vertical.cpp \
    src/sound_recorder_exception.cpp \
    src/tools/utilities.cpp \
    src/tools/wavfile.cpp

HEADERS  += mainwindow.h \
    include/private/entities/audio_spectre.h \
    include/private/entities/settings_dialog.h \
    include/private/entities/sound_engine.h \
    include/private/entities/sound_track.h \
    include/private/entities/spectre_container.h \
    include/private/factories/sound_tracks.h \
    include/private/levels/level.h \
    include/private/levels/level_horizontal.h \
    include/private/levels/level_vertical.h \
    include/public/entities/i_audio_spectre.h \
    include/public/entities/i_settings_dialog.h \
    include/public/entities/i_sound_engine.h \
    include/public/entities/i_sound_track.h \
    include/public/entities/i_spectre_container.h \
    include/public/factories/i_sound_tracks.h \
    include/public/levels/i_level.h \
    include/public/sound_recorder_exception.h \
    include/public/tools/utilities.h \
    include/public/tools/wavfile.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc
