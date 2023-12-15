include(../plugins.pri)

MODBUS_CONNECTIONS += amperfied-registers.json

MODBUS_TOOLS_CONFIG += VERBOSE

include(../modbus.pri)

HEADERS += \
    amperfiedconnectdiscovery.h \
    energycontroldiscovery.h \
    integrationpluginamperfied.h

SOURCES += \
    amperfiedconnectdiscovery.cpp \
    energycontroldiscovery.cpp \
    integrationpluginamperfied.cpp
