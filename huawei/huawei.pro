include(../plugins.pri)

# Generate modbus connection
#MODBUS_TOOLS_CONFIG += VERBOSE
MODBUS_CONNECTIONS += \
    huawei-fusion-solar-registers.json \
    huawei-registers.json \
    smartlogger-registers.json \

include(../modbus.pri)

HEADERS += \
    huasweismartloggerdiscovery.h \
    huaweifusionsolar.h \
    huaweifusionsolardiscovery.h \
    integrationpluginhuawei.h

SOURCES += \
    huasweismartloggerdiscovery.cpp \
    huaweifusionsolar.cpp \
    huaweifusionsolardiscovery.cpp \
    integrationpluginhuawei.cpp
