include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += amtron-ecu-registers.json \
                      amtron-hcc3-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    amtronecudiscovery.h \
    amtronhcc3discovery.h \
    integrationpluginmennekes.h

SOURCES += \
    amtronecudiscovery.cpp \
    amtronhcc3discovery.cpp \
    integrationpluginmennekes.cpp
