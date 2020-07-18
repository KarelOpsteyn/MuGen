TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Dependencies/Binasc.cpp \
    Dependencies/MidiEvent.cpp \
    Dependencies/MidiEventList.cpp \
    Dependencies/MidiFile.cpp \
    Dependencies/MidiMessage.cpp \
    Dependencies/Options.cpp \
    note.cpp \
    scale.cpp \
    song.cpp \
    grade.cpp

HEADERS += \
    Dependencies/Binasc.h \
    Dependencies/MidiEvent.h \
    Dependencies/MidiEventList.h \
    Dependencies/MidiFile.h \
    Dependencies/MidiMessage.h \
    Dependencies/Options.h \
    note.h \
    scale.h \
    song.h \
    grade.h
