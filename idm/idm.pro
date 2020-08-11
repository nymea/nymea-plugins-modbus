include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    idm.cpp \
    integrationpluginidm.cpp \
    ../modbus/modbustcpmaster.cpp \

HEADERS += \
    idm.h \
    integrationpluginidm.h \
    ../modbus/modbustcpmaster.h \
