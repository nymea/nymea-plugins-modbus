include(../plugins.pri)

MODBUS_CONNECTIONS += pantabox-registers.json

MODBUS_TOOLS_CONFIG += VERBOSE

include(../modbus.pri)

HEADERS += \
    crclookuptable.h \
    integrationplugininro.h \
    pantabox.h \
    pantaboxdiscovery.h \
    pantaboxudpdiscovery.h

SOURCES += \
    integrationplugininro.cpp \
    pantabox.cpp \
    pantaboxdiscovery.cpp \
    pantaboxudpdiscovery.cpp
