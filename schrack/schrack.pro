include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += cion-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

SOURCES += \
    integrationpluginschrack.cpp

HEADERS += \
    integrationpluginschrack.h

