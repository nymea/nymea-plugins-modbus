include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += growatt-registers.json

#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationplugingrowatt.h

SOURCES += \
    integrationplugingrowatt.cpp
