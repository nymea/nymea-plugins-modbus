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
    neuron/neuron.cpp \
    neuronextension/neuronextension.cpp \
    neuronextension/neuronextensionbus.cpp \
    neuronextension/neuronextensiondiscovery.cpp \
    unipi/i2cport.cpp \
    unipi/mcp23008.cpp \
    unipi/mcp342xchannel.cpp \
    unipi/unipi.cpp \
    unipi/unipipwm.cpp

HEADERS += \
    integrationpluginunipi.h \
    neuron/neuron.h \
    neuronextension/neuronextension.h \
    neuronextension/neuronextensionbus.h \
    neuronextension/neuronextensiondiscovery.h \
    unipi/i2cport.h \
    unipi/i2cport_p.h \
    unipi/mcp23008.h \
    unipi/mcp342xchannel.h \
    unipi/unipi.h \
    unipi/unipipwm.h

MAP_FILES.files = modbus_maps/*
MAP_FILES.path = /usr/share/nymea/modbus/
INSTALLS += MAP_FILES
