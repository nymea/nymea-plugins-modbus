include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += pro380-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationplugininepro.h

SOURCES += \
    integrationplugininepro.cpp

