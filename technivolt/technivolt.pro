include(../plugins.pri)

MODBUS_CONNECTIONS += technivolt-registers.json
include(../modbus.pri)

SOURCES += \
    integrationplugintechnivolt.cpp \
    technivoltwallbox.cpp

HEADERS += \
    integrationplugintechnivolt.h \
    technivoltwallbox.h
