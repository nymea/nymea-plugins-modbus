include(../plugins.pri)

QT += \
    serialport \
    network \
    serialbus \

SOURCES += \
    integrationpluginmodbuscommander.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../modbus/modbusrtumaster.cpp \

HEADERS += \
    integrationpluginmodbuscommander.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusrtumaster.h \

