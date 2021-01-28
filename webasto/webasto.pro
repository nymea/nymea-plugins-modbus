include(../plugins.pri)

QT += \
    serialbus \
    network

SOURCES += \
    integrationpluginwebasto.cpp \
    webasto.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../discovery/discovery.cpp \
    ../discovery/host.cpp

HEADERS += \
    integrationpluginwebasto.h \
    webasto.h \
    ../modbus/modbustcpmaster.h \
    ../discovery/discovery.h \
    ../discovery/host.h
