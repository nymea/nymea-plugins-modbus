include(../plugins.pri)

QT += serialport serialbus

HEADERS += \
    integrationplugininepro.h \
    pro380modbusrtuconnection.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    integrationplugininepro.cpp \
    pro380modbusrtuconnection.cpp \
    ../modbus/modbusdatautils.cpp

