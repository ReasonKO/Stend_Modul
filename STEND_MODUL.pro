#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T20:52:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STEND_MODUL
TEMPLATE = app



SHARED_DIR = ../macsCommon
LIB_DIR = ../lib
DESTDIR = ../Stend_Modul_build$${SUFFIX_STR}
MATLAB_DIR = 'C:/Program Files (x86)/MATLAB/R2014a/extern/'

win32 {
  #add libs
  LIBS += -lws2_32

  LIBS += -L$${MATLAB_DIR}/lib/win32/microsoft/ -llibeng \
          -L$${MATLAB_DIR}/lib/win32/microsoft/ -llibmat \
          -L$${MATLAB_DIR}/lib/win32/microsoft/ -llibmx
}


INCLUDEPATH += \
  $${MATLAB_DIR}/include \

SOURCES += main.cpp\
        mainwindow.cpp \
    mlwork.cpp \
    mldata.cpp \
    remotecontrol.cpp

HEADERS  += mainwindow.h \
    mlwork.h \
    mldata.h \
    remotecontrol.h

FORMS    += mainwindow.ui \
    remotecontrol.ui
