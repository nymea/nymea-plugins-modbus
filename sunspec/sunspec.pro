include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    integrationpluginsunspec.cpp \
    sunspec.cpp \
    ../modbus/modbustcpmaster.cpp \

HEADERS += \
    integrationpluginsunspec.h \
    sunspec.h \
    ../modbus/modbustcpmaster.h \
