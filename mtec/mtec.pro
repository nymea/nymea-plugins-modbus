include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    mtec.cpp \
    mtechelpers.cpp \
    integrationpluginmtec.cpp \
    ../modbus/modbustcpmaster.cpp

HEADERS += \
    mtec.h \
    mtechelpers.h \
    mtecinfo.h \
    integrationpluginmtec.h \
    ../modbus/modbustcpmaster.h \

