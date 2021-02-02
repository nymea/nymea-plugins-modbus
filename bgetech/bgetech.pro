include(../plugins.pri)

QT += \ 
    serialport \
    serialbus \

SOURCES += \
    integrationpluginbgetech.cpp \
    ../modbus/modbusrtumaster.cpp \

HEADERS += \
    integrationpluginbgetech.h \
    ../modbus/modbusrtumaster.h \
