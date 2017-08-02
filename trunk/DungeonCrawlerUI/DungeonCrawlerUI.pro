#-------------------------------------------------
#
# Project created by QtCreator 2017-07-13T12:16:16
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DungeonCrawlerUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    project.cpp \
    map.cpp \
    tile.cpp \
    logger.cpp \
    homescreen.cpp \
    flatbutton.cpp \
    mapview.cpp \
    mapwindow.cpp \
    tileset.cpp \
    tileimage.cpp \
    tileindex.cpp

HEADERS  += mainwindow.h \
    project.h \
    map.h \
    tile.h \
    logger.h \
    homescreen.h \
    flatbutton.h \
    mapview.h \
    mapwindow.h \
    tooltypes.h \
    tileset.h \
    tileimage.h \
    tileindex.h

FORMS    += mainwindow.ui \
    homescreen.ui \
    mapview.ui

DISTFILES += \
    config.txt

