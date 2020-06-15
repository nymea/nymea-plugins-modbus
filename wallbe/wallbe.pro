include(../plugins.pri)

QT += \ 
    network \
    serialbus \

SOURCES += \
    integrationpluginwallbe.cpp \
    wallbe.cpp \
    host.cpp \
    discover.cpp

HEADERS += \
    integrationpluginwallbe.h \
    wallbe.h \
    host.h \
    discover.h
