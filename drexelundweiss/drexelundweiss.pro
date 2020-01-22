include(../plugins.pri)

TARGET = $$qtLibraryTarget(nymea_deviceplugindrexelundweiss)

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


