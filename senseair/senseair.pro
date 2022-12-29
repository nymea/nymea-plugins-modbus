include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += s8-registers.json 
MODBUS_TOOLS_CONFIG += VERBOSE

include(../modbus.pri)

HEADERS += \
    integrationpluginsenseair.h

SOURCES += \
    integrationpluginsenseair.cpp
