#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T17:32:04
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = min
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += /usr/include/lua5.2
LIBS += -lgsl -lgslcblas -lluabind -llua5.2 -lconfig++ -lGL -lGLU -lglut -lboost_program_options

TEMPLATE = app

HEADERS += array.h \
            bspincuboid.h \
            vis/arrow.h \
            vis/glwidget.h \
            vis/mainwindow.h \
           configurable.h \
           configuration.h \
            defectcounter.h \
           defectedhelical.h \
           exception.h \
            functional.h \
            ginzburglandau.h \
            ginzburglandaupinned.h \
            ginzburglandau2pinned.h \
            helical.h \
            interpolated.h \
           luainterpreter.h \
            luathread.h \
            minimizer.h \
            mathematicareader.h \
            mathematicawriter.h \
           named.h \
           phase.h \
           polarizer.h \
           skyrmionlattice.h \
           skyrmionizer.h \
           spincuboid.h \
           tool.h \
           utils.h \
            viewer.h \
    noise.h \
    polarized.h

SOURCES += array.cpp \
            bspincuboid.cpp \
            vis/arrow.cpp \
            vis/glwidget.cpp \
            vis/mainwindow.cpp \
            configurable.cpp \
            configuration.cpp \
            defectcounter.cpp \
            defectedhelical.cpp \
            exception.cpp \
            functional.cpp \
            ginzburglandau.cpp \
            ginzburglandaupinned.cpp \
            ginzburglandau2pinned.cpp \
            helical.cpp \
            interpolated.cpp \
            luainterpreter.cpp \
            luathread.cpp \
            main.cpp \
            minimizer.cpp \
            mathematicareader.cpp \
            mathematicawriter.cpp \
            named.cpp \
            phase.cpp \
            polarizer.cpp \
            skyrmionlattice.cpp \
            skyrmionizer.cpp \
            spincuboid.cpp \
            tool.cpp \
            utils.cpp \
            viewer.cpp \
    noise.cpp \
    polarized.cpp

FORMS += vis/mainwindow.ui
