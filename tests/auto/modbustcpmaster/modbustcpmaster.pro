QT += core network serialbus testlib
QT -= gui

CONFIG += testcase console
CONFIG -= app_bundle

TARGET = tst_modbustcpmaster

INCLUDEPATH += ../../../libnymea-modbus
LIBS += -L$$shadowed($$PWD/../../../libnymea-modbus) -lnymea-modbus

SOURCES += tst_modbustcpmaster.cpp

RESOURCES += testdata.qrc
