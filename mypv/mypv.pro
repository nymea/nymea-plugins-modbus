include(../plugins.pri)

TARGET = $$qtLibraryTarget(nymea_devicepluginmypv)

QT += \
    network \
    serialbus \

LIBS += -lmodbus

SOURCES += \
    devicepluginmypv.cpp \
    modbustcpmaster.cpp \

HEADERS += \
    devicepluginmypv.h \
    modbustcpmaster.h \


