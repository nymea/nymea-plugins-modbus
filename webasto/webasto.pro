include(../plugins.pri)

QT += \
    serialbus \
    network

SOURCES += \
    integrationpluginwebasto.cpp \
    webasto.cpp \
    ../discovery/discovery.cpp \
    ../discovery/host.cpp

HEADERS += \
    integrationpluginwebasto.h \
    webasto.h \
    ../discovery/discovery.h \
    ../discovery/host.h
