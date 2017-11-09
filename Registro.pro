#-------------------------------------------------
#
# Project created by QtCreator 2017-11-03T16:18:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Registro
TEMPLATE = app
LIBS += -lsqlite3

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        registro_main_gui.cpp \
    addrecord.cpp \
    editrecord.cpp \
    about.cpp \
    db_connection.cpp \
    adddb.cpp \
    subjects.cpp

HEADERS += \
        registro_main_gui.h \
    adddb.h \
    addrecord.h \
    editrecord.h \
    about.h \
    db_connection.h \ 
    subjects.h

FORMS += \
        registro_main_gui.ui \
    about.ui \
    adddb.ui \
    addrecord.ui \
    editrecord.ui \
    registro_main_gui.ui \
    subjects.ui

RESOURCES += \
    resources.qrc
