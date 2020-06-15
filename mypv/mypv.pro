include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    integrationpluginmypv.cpp \
    modbustcpmaster.cpp \

HEADERS += \
    integrationpluginmypv.h \
    modbustcpmaster.h \


