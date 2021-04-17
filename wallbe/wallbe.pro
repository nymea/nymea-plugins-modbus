include(../plugins.pri)

QT += \ 
    network \
    serialbus \

SOURCES += \
    integrationpluginwallbe.cpp \
    ../modbus/modbustcpmaster.cpp \
    discovery.cpp \
    host.cpp

HEADERS += \
    integrationpluginwallbe.h \
    ../modbus/modbustcpmaster.h \
    discovery.h \
    host.h
