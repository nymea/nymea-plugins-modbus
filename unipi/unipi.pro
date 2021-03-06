include(../plugins.pri)

LIBS += \
    -li2c \
    -lmodbus

INCLUDEPATH += modbus

PKGCONFIG += nymea-gpio

QT += \
    network \
    serialport \

SOURCES += \
    integrationpluginunipi.cpp \
    neuron.cpp \
    mcp23008.cpp \
    i2cport.cpp \
    unipi.cpp \
    mcp342xchannel.cpp \
    unipipwm.cpp \
    neuronextension/neuronextension.cpp \
    neuronextension/neuronextensionbus.cpp \
    neuronextension/neuronextensiondiscovery.cpp

HEADERS += \
    integrationpluginunipi.h \
    neuron.h \
    mcp23008.h \
    i2cport.h \
    unipi.h \
    i2cport_p.h \
    mcp342xchannel.h \
    unipipwm.h \
    neuronextension/neuronextension.h \
    neuronextension/neuronextensionbus.h \
    neuronextension/neuronextensiondiscovery.h

MAP_FILES.files = files(modbus_maps/*)
MAP_FILES.path = [QT_INSTALL_PREFIX]/share/nymea/modbus/
INSTALLS += MAP_FILES

