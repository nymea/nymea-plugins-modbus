include(../plugins.pri)

MODBUS_CONNECTIONS=phoenixconnect-registers.json
include(../modbus.pri)

SOURCES += \
    integrationpluginphoenixconnect.cpp \
    phoenixdiscovery.cpp

HEADERS += \
    integrationpluginphoenixconnect.h \
    phoenixdiscovery.h
