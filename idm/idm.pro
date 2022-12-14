include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += idm-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

SOURCES += \
    integrationpluginidm.cpp

HEADERS += \
    integrationpluginidm.h
