include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += amtron-ecu-registers.json \
                      amtron-hcc3-registers.json \
                      amtron-compact20-registers.json

#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    amtroncompact20discovery.h \
    amtronecu.h \
    amtronecudiscovery.h \
    amtronhcc3discovery.h \
    integrationpluginmennekes.h

SOURCES += \
    amtroncompact20discovery.cpp \
    amtronecu.cpp \
    amtronecudiscovery.cpp \
    amtronhcc3discovery.cpp \
    integrationpluginmennekes.cpp
