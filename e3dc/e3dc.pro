include(../plugins.pri)

QT += network serialbus

SOURCES += \
    e3dcbattery.cpp \
    e3dcinverter.cpp \
    e3dcsmartmeter.cpp \
    e3dcwallbox.cpp \
    integrationplugine3dc.cpp \
    tcp_modbusconnection.cpp \
    ../libnymea-modbus/modbustcpmaster.cpp \
    ../libnymea-modbus/modbusdatautils.cpp

HEADERS += \
    e3dcbattery.h \
    e3dcinverter.h \
    e3dcsmartmeter.h \
    e3dcwallbox.h \
    integrationplugine3dc.h \
    tcp_modbusconnection.h \
    ../libnymea-modbus/modbustcpmaster.h \
    ../libnymea-modbus/modbusdatautils.h
