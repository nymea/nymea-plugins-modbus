include(../plugins.pri)

QT += serialport serialbus

HEADERS += \
    integrationpluginenergymeters.h \
    sdm630modbusrtuconnection.h \
    pro380modbusrtuconnection.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    integrationpluginenergymeters.cpp \
    sdm630modbusrtuconnection.cpp \
    pro380modbusrtuconnection.cpp \
    ../modbus/modbusdatautils.cpp

