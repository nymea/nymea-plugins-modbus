include(../plugins.pri)

QT += \ 
    serialport \
    serialbus \

SOURCES += \
    energymeter.cpp \
    integrationpluginenergymeters.cpp

HEADERS += \
    energymeter.h \
    integrationpluginenergymeters.h \
    inepromodbusregister.h \
    bg-etechmodbusregister.h \
    registerdescriptor.h
