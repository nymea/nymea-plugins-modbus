include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += wattsonic-registers.json

#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    wattsonicdiscovery.h \
    integrationpluginwattsonic.h

SOURCES += \
    wattsonicdiscovery.cpp \
    integrationpluginwattsonic.cpp
