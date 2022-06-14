include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += stiebel-eltron-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationpluginstiebeleltron.h

SOURCES += \
    integrationpluginstiebeleltron.cpp

