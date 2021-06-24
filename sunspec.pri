QT += network serialbus

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

INCLUDEPATH += libnymea-sunspec
LIBS += -L$$top_builddir/libnymea-sunspec/ -lnymea-sunspec1
