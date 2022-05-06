include(../plugins.pri)

# Generate modbus connection
MODBUS_CONNECTIONS += huawei-registers.json
#MODBUS_TOOLS_CONFIG += VERBOSE
include(../modbus.pri)

HEADERS += \
    huaweifusionsolar.h \
    integrationpluginhuawei.h

SOURCES += \
    huaweifusionsolar.cpp \
    integrationpluginhuawei.cpp
