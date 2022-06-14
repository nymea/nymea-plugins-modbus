QMAKE_CXXFLAGS += -Werror -std=c++11 -z defs
QMAKE_LFLAGS += -std=c++11 -z defs

QT += network serialbus

CONFIG += link_pkgconfig
PKGCONFIG += nymea

TARGET = nymea-modbus
TEMPLATE = lib

gcc {
    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    COMPILER_MAJOR_VERSION = $$str_member($$COMPILER_VERSION)
    greaterThan(COMPILER_MAJOR_VERSION, 7): QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

HEADERS += \
    modbusdatautils.h \
    modbustcpmaster.h

SOURCES += \
    modbusdatautils.cpp \
    modbustcpmaster.cpp


# define install target
target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

# install modbustool for external plugins
modbustoolpri.files = modbus-tool.pri
modbustoolpri.path = $$[QT_INSTALL_PREFIX]/include/nymea-modbus/
modbustool.files = tools/generate-connection.py
modbustool.path = $$[QT_INSTALL_PREFIX]/include/nymea-modbus/tools/
modbustoolmodules.files = tools/connectiontool/*.py
modbustoolmodules.path = $$[QT_INSTALL_PREFIX]/include/nymea-modbus/tools/connectiontool/
INSTALLS += modbustoolpri modbustool modbustoolmodules

# install header file with relative subdirectory
for (header, HEADERS) {
    path = $$[QT_INSTALL_PREFIX]/include/nymea-modbus/$${dirname(header)}
    eval(headers_$${path}.files += $${header})
    eval(headers_$${path}.path = $${path})
    eval(INSTALLS *= headers_$${path})
}

# Create pkgconfig file
CONFIG += create_pc create_prl no_install_prl
QMAKE_PKGCONFIG_NAME = libnymea-modbus
QMAKE_PKGCONFIG_DESCRIPTION = nymea modbus integrations development library
QMAKE_PKGCONFIG_PREFIX = $$[QT_INSTALL_PREFIX]
QMAKE_PKGCONFIG_INCDIR = $$[QT_INSTALL_PREFIX]/include/nymea-modbus/
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_VERSION = 1.0.0
QMAKE_PKGCONFIG_FILE = nymea-modbus
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
