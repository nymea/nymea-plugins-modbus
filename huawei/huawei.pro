include(../plugins.pri)

QT += network serialbus

HEADERS += \
    huaweifusionsolar.h \
    huaweimodbustcpconnection.h \
    integrationpluginhuawei.h \
    ../modbus/modbustcpmaster.h \
    ../modbus/modbusdatautils.h

SOURCES += \
    huaweifusionsolar.cpp \
    huaweimodbustcpconnection.cpp \
    integrationpluginhuawei.cpp \
    ../modbus/modbustcpmaster.cpp \
    ../modbus/modbusdatautils.cpp
