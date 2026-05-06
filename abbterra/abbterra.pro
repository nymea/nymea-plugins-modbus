include(../plugins.pri)

MODBUS_CONNECTIONS += abbterra-registers.json
MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

TARGET = $$qtLibraryTarget(nymea_integrationpluginabbterra)

OTHER_FILES += abbterra-registers.json

SOURCES += \
    abbterrartudiscovery.cpp \
    abbterratcpdiscovery.cpp \
    integrationpluginabbterra.cpp

HEADERS += \
    abbterrartudiscovery.h \
    abbterratcpdiscovery.h \
    abbterrautils.h \
    integrationpluginabbterra.h
