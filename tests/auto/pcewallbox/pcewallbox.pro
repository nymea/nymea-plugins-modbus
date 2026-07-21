QT += core network serialbus testlib
QT -= gui

CONFIG += testcase console no_testcase_installs
CONFIG -= app_bundle

TARGET = tst_pcewallbox

INCLUDEPATH += \
    . \
    ../../../pcelectric \
    ../../../libnymea-modbus

LIBS += -L$$shadowed($$PWD/../../../libnymea-modbus) -lnymea-modbus

MODBUS_CONNECTIONS += ../../../pcelectric/EV11.3-registers.json
include(../../../libnymea-modbus/modbus-tool.pri)

HEADERS += \
    extern-plugininfo.h \
    ../../../pcelectric/pcewallbox.h

SOURCES += \
    tst_pcewallbox.cpp \
    ../../../pcelectric/pcewallbox.cpp
