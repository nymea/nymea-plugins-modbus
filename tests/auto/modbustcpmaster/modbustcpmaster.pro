QT += core network serialbus testlib
QT -= gui

CONFIG += testcase console no_testcase_installs
CONFIG -= app_bundle

TARGET = tst_modbustcpmaster

INCLUDEPATH += ../../../libnymea-modbus
LIBS += -L$$shadowed($$PWD/../../../libnymea-modbus) -lnymea-modbus

SOURCES += tst_modbustcpmaster.cpp

RESOURCES += testdata.qrc

packagesExist(openssl) {
    CONFIG += link_pkgconfig
    PKGCONFIG += openssl
    DEFINES += HAVE_OPENSSL_TEST_SERVER
}
