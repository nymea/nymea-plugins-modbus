include(../plugins.pri)

QT += \
    serialport \
    serialbus \

SOURCES += \
    integrationplugindrexelundweiss.cpp \
    modbusrtumaster.cpp \

HEADERS += \
    integrationplugindrexelundweiss.h \
    modbusrtumaster.h \
    modbusdegisterdefinition.h


