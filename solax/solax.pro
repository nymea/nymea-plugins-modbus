include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += solax-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationpluginsolax.h \
    solaxdiscovery.h

SOURCES += \
    integrationpluginsolax.cpp \
    solaxdiscovery.cpp
