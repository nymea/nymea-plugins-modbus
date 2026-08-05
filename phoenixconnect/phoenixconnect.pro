include(../plugins.pri)

MODBUS_CONNECTIONS = phoenixconnect-registers.json \
    eboxprofessional-registers.json
include(../modbus.pri)

SOURCES += \
    integrationpluginphoenixconnect.cpp \
    eboxprofessionaldiscovery.cpp \
    phoenixdiscovery.cpp

HEADERS += \
    integrationpluginphoenixconnect.h \
    eboxprofessionaldiscovery.h \
    phoenixdiscovery.h
