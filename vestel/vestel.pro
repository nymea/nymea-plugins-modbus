include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += evc04-registers.json

#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    evc04discovery.h \
    integrationpluginvestel.h

SOURCES += \
    evc04discovery.cpp \
    integrationpluginvestel.cpp
