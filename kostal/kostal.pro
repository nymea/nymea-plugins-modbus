include(../plugins.pri)

QT += network serialbus

SOURCES += \
    integrationpluginkostal.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../modbus/modbusdatautils.cpp \
    kostalconnection.cpp

HEADERS += \
    integrationpluginkostal.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusdatautils.h \
    kostalconnection.h
