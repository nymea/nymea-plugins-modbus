include(../plugins.pri)

QT += \
    serialport \
    network \
    serialbus \

SOURCES += \
    integrationpluginmodbuscommander.cpp \
    ../modbus/modbustcpmaster.cpp \

HEADERS += \
    integrationpluginmodbuscommander.h \
    ../modbus/modbustcpmaster.h \

