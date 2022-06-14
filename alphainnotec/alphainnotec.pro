include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += alphainnotec-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

SOURCES += \
    integrationpluginalphainnotec.cpp

HEADERS += \
    integrationpluginalphainnotec.h
