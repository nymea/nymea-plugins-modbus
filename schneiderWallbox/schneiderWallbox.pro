include(../plugins.pri)

MODBUS_CONNECTIONS += schneiderwallbox-registers.json
include(../modbus.pri)

SOURCES += \
    integrationpluginschneiderwallbox.cpp \
    schneiderwallbox.cpp

HEADERS += \
    integrationpluginschneiderwallbox.h \
    schneiderwallbox.h
