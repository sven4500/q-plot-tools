QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graph2d
TEMPLATE = app

include(../defs.pri)

SOURCES += \
    main.cpp

HEADERS += \
    mainwidget.h

