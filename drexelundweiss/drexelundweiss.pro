include(../plugins.pri)

QT += \
    serialport \
    serialbus \

SOURCES += \
    deviceplugindrexelundweiss.cpp \
    modbusrtumaster.cpp \

HEADERS += \
    deviceplugindrexelundweiss.h \
    modbusrtumaster.h \
    modbusdegisterdefinition.h


