QT += core
QT -= gui

CONFIG += c++11

TARGET = taquin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    puzzlestate.cpp \
    puzzlesolver.cpp

HEADERS += \
    puzzlestate.h \
    puzzlesolver.h
