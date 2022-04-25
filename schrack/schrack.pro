include(../plugins.pri)

QT += serialport serialbus

SOURCES += \
    integrationpluginschrack.cpp \
    cionmodbusrtuconnection.cpp \
    ../modbus/modbusdatautils.cpp

HEADERS += \
    integrationpluginschrack.h \
    cionmodbusrtuconnection.h \
    ../modbus/modbusdatautils.h

OTHER_FILES += cion-registers.json

modbusconnection.commands = python $${top_srcdir}/modbus/tools/generate-connection.py -j $${_PRO_FILE_PWD_}/cion-registers.json -o $${_PRO_FILE_PWD_} -c CionModbusRtuConnection
QMAKE_EXTRA_TARGETS += modbusconnection

#target.depends += modbusconnection
