QT += core serialbus testlib
QT -= gui

CONFIG += testcase console no_testcase_installs
CONFIG -= app_bundle

TARGET = tst_modbuscli

INCLUDEPATH += ../../../nymea-modbus-cli

SOURCES += \
    tst_modbuscli.cpp \
    ../../../nymea-modbus-cli/registerdatautils.cpp

HEADERS += ../../../nymea-modbus-cli/registerdatautils.h
