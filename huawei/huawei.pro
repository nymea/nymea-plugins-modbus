include(../plugins.pri)

# Generate modbus connection
#MODBUS_TOOLS_CONFIG += VERBOSE
MODBUS_CONNECTIONS += \
    huawei-fusion-solar-registers.json \
    huawei-registers.json \
    smartlogger-registers.json \

include(../modbus.pri)

HEADERS += \
    huaweifusionsolar.h \
    huaweifusionsolardiscovery.h \
    huaweismartloggerdiscovery.h \
    integrationpluginhuawei.h

SOURCES += \
    huaweifusionsolar.cpp \
    huaweifusionsolardiscovery.cpp \
    huaweismartloggerdiscovery.cpp \
    integrationpluginhuawei.cpp
