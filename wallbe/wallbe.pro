include(../plugins.pri)

QT += \ 
    network \
    serialbus \

SOURCES += \
    integrationpluginwallbe.cpp \
    ../modbus/modbustcpmaster.cpp \
    host.cpp \
    discover.cpp

HEADERS += \
    integrationpluginwallbe.h \
    ../modbus/modbustcpmaster.h \
    host.h \
    discover.h
