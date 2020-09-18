include(../plugins.pri)

QT += \
    network \
    serialbus \

SOURCES += \
    integrationpluginsunspec.cpp \
    sunspec.cpp \
    sunspecinverter.cpp \
    sunspecmeter.cpp \
    sunspecstorage.cpp \
    sunspectracker.cpp \
    sunspecstringcombiner.cpp \

HEADERS += \
    integrationpluginsunspec.h \
    sunspec.h \
    sunspecinverter.h \
    sunspecmeter.h \
    sunspecstorage.h \
    sunspectracker.h \
    sunspecstringcombiner.h \
