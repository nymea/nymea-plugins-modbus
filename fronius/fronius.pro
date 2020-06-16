include(../plugins.pri)

QT += \
    serialbus \
    network \

SOURCES += \
    integrationpluginfronius.cpp \
    froniusthing.cpp \
    froniuslogger.cpp \
    froniusinverter.cpp \
    froniusstorage.cpp \
    froniusmeter.cpp \
    sunspecthing.cpp \
    ../modbus/modbustcpmaster.h \

HEADERS += \
    integrationpluginfronius.h \
    froniusthing.h \
    froniuslogger.h \
    froniusinverter.h \
    froniusstorage.h \
    froniusmeter.h \
    sunspecthing.h \
    ../modbus/modbustcpmaster.h \
