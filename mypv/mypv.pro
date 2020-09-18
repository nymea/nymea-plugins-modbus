include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    integrationpluginmypv.cpp \
    ../modbus/modbustcpmaster.cpp \

HEADERS += \
    integrationpluginmypv.h \
    ../modbus/modbustcpmaster.h \
