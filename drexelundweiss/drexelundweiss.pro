include(../plugins.pri)

QT += \
    serialport \
    serialbus \

SOURCES += \
    integrationplugindrexelundweiss.cpp \
    ../modbus/modbusrtumaster.cpp \

HEADERS += \
    integrationplugindrexelundweiss.h \
    modbusdegisterdefinition.h \
    ../modbus/modbusrtumaster.h \
