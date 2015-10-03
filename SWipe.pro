#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T22:07:59
#
#-------------------------------------------------

QT += core gui multimedia
QT += widgets

TARGET = SWipe
TEMPLATE = app


SOURCES += main.cpp\
        swipe.cpp \
    magdecode.cpp \
    mslib.c \
    llist.c \
    carddetect.cpp

HEADERS  += swipe.h \
    magdecode.h \
    mslib.h \
    llist.h \
    carddetect.h \
    magcard.h

CONFIG += 
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe9952b07
    TARGET.CAPABILITY += "UserEnvironment"
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
