include(../plugins.pri)

QT += network serialbus

HEADERS += \
    integrationpluginstiebeleltron.h \
    stiebeleltronmodbusconnection.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    integrationpluginstiebeleltron.cpp \
    stiebeleltronmodbusconnection.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../modbus/modbusdatautils.cpp

