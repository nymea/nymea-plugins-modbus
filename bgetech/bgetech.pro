include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += sdm630-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationpluginbgetech.h

SOURCES += \
    integrationpluginbgetech.cpp

