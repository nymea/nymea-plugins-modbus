include(../plugins.pri)

QT += network serialbus

SOURCES += \
    integrationpluginalphainnotec.cpp \
    alphaconnectmodbustcpconnection.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../modbus/modbusdatautils.cpp

HEADERS += \
    integrationpluginalphainnotec.h \
    alphaconnectmodbustcpconnection.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusdatautils.h
