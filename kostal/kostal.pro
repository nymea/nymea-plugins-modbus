include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += kostal-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationpluginkostal.h \
    kostaldiscovery.h

SOURCES += \
    integrationpluginkostal.cpp \
    kostaldiscovery.cpp
