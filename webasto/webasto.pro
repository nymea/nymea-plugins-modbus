include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += webasto-next-registers.json
MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

SOURCES += \
    integrationpluginwebasto.cpp \
    webasto.cpp \
    webastodiscovery.cpp

HEADERS += \
    integrationpluginwebasto.h \
    webasto.h \
    webastodiscovery.h
