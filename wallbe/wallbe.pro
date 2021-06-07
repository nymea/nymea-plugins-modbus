include(../plugins.pri)

QT += \ 
    network \
    serialbus \

SOURCES += \
    integrationpluginwallbe.cpp \
    ../modbus/modbustcpmaster.cpp

HEADERS += \
    integrationpluginwallbe.h \
    ../modbus/modbustcpmaster.h
