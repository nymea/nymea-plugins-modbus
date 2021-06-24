include(../plugins.pri)

QT += serialbus network

SOURCES += \
    integrationpluginwebasto.cpp \
    webasto.cpp \
    ../modbus/modbustcpmaster.cpp

HEADERS += \
    integrationpluginwebasto.h \
    webasto.h \
    ../modbus/modbustcpmaster.h
