QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = voronoidiagram
TEMPLATE = app

include(../defs.pri)

SOURCES += \
    main.cpp

HEADERS += \
    widget.h
