# Project File Created by icebit


QT		+= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Resistor
TEMPLATE = app

SOURCES += main.cpp\
		about.cpp \
		adddb.cpp \
		addrecord.cpp \
		db_connection.cpp \
		editrecord.cpp \
	registro_main_gui.cpp 
	
HEADERS += main.h\
		about.h \
		adddb.h \
		addrecord.h \
		db_connection.h \
		editrecord.h \
	registro_main_gui.h 
	
FORMS += about.ui \
		 adddb.ui \
		 addrecord.ui \
		 editrecord.ui \
		 registro_main_gui.ui
		

