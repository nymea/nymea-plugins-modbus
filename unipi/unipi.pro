include(../plugins.pri)

LIBS += \
    -li2c \

PKGCONFIG += nymea-gpio

QT += \
    network \
    serialport \
    serialbus \

SOURCES += \
    integrationpluginunipi.cpp \
    neuron.cpp \
    neuroncommon.cpp \
    neuronextension.cpp \
    mcp23008.cpp \
    i2cport.cpp \
    unipi.cpp \
    mcp342xchannel.cpp \
    unipipwm.cpp

HEADERS += \
    integrationpluginunipi.h \
    neuron.h \
    neuroncommon.h \
    neuronextension.h \
    mcp23008.h \
    i2cport.h \
    unipi.h \
    i2cport_p.h \
    mcp342xchannel.h \
    unipipwm.h

MAP_FILES.files = files(modbus_maps/*)
MAP_FILES.path = [QT_INSTALL_PREFIX]/share/nymea/modbus/
INSTALLS += MAP_FILES

