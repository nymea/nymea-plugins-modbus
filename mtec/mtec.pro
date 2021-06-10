include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    mtec.cpp \
    integrationpluginmtec.cpp \
    ../modbus/modbustcpmaster.cpp

HEADERS += \
    mtec.h \
    integrationpluginmtec.h \
    ../modbus/modbustcpmaster.h \

