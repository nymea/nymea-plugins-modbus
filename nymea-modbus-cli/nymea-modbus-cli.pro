TARGET = nymea-modbus-cli

QT += network serialport serialbus
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS *= -Werror -std=c++11 -g
QMAKE_LFLAGS *= -std=c++11

gcc {
    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    COMPILER_MAJOR_VERSION = $$str_member($$COMPILER_VERSION)
    greaterThan(COMPILER_MAJOR_VERSION, 7): QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

SOURCES += \
        main.cpp

target.path = $$[QT_INSTALL_PREFIX]/bin
INSTALLS += target
