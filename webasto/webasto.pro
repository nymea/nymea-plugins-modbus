include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += webasto-next-registers.json ../vestel/evc04-registers.json
MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

SOURCES += \
    integrationpluginwebasto.cpp \
    webasto.cpp \
    webastodiscovery.cpp \
    ../vestel/evc04discovery.cpp

HEADERS += \
    integrationpluginwebasto.h \
    webasto.h \
    webastodiscovery.h \
    ../vestel/evc04discovery.h
