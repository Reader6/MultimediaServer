TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        dohttp.c \
        main.c \
        msocket.c \
        tpool.c

HEADERS += \
    dohttp.h \
    msocket.h \
    tpool.h

LIBS += -lpthread
