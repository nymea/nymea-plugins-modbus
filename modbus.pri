QT += network serialport serialbus

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

INCLUDEPATH += $$top_srcdir/libnymea-modbus
LIBS += -L$$top_builddir/libnymea-modbus/ -lnymea-modbus

OTHER_FILES += $${MODBUS_CONNECTIONS}

include(libnymea-modbus/modbus-tool.pri)

