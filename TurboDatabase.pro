QT += core
QT -= gui

CONFIG += c++11

TARGET = TurboDatabase
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    index.cpp \
    btreeindex.cpp \
    database.cpp \
    table.cpp \
    record.cpp

HEADERS += \
    index.h \
    btreeindex.h \
    database.h \
    table.h \
    record.h

DISTFILES += \
    save
