include(../plugins.pri)

QT += serialport serialbus

HEADERS += \
    integrationpluginbgetech.h \
    sdm630modbusrtuconnection.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    integrationpluginbgetech.cpp \
    sdm630modbusrtuconnection.cpp \
    ../modbus/modbusdatautils.cpp

