include(../plugins.pri)

# Generate modbus connections
MODBUS_CONNECTIONS += bender-tqdm100-registers.json
MODBUS_CONNECTIONS += bender-omcci-registers.json

#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    integrationpluginbender.h \
    omccidiscovery.h \
    tqdm100discovery.h

SOURCES += \
    integrationpluginbender.cpp \
    omccidiscovery.cpp \
    tqdm100discovery.cpp
