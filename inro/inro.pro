include(../plugins.pri)

MODBUS_CONNECTIONS += pantabox-registers.json

MODBUS_TOOLS_CONFIG += VERBOSE

include(../modbus.pri)

HEADERS += \
    integrationplugininro.h \
    pantabox.h \
    pantaboxdiscovery.h

SOURCES += \
    integrationplugininro.cpp \
    pantabox.cpp \
    pantaboxdiscovery.cpp
