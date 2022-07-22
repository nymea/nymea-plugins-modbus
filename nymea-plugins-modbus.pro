TEMPLATE = subdirs

# Note: In the loop at the end of this file the plugin
# dependency on the libs will be defined
SUBDIRS += nymea-modbus-cli libnymea-modbus libnymea-sunspec

PLUGIN_DIRS = \
    alphainnotec            \
    bgetech                 \
    drexelundweiss          \
    huawei                  \
    idm                     \
    inepro                  \
    modbuscommander         \
    mtec                    \
    mypv                    \
    phoenixconnect          \
    schrack                 \
    stiebeleltron           \
    sunspec                 \
    unipi                   \
    webasto                 \

gcc {
    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    COMPILER_MAJOR_VERSION = $$str_member($$COMPILER_VERSION)
    greaterThan(COMPILER_MAJOR_VERSION, 7): QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

message(============================================)
message("Qt version:" $$[QT_VERSION])

plugininfo.depends = FORCE
for (entry, PLUGIN_DIRS):plugininfo.commands += test -d $${entry} || mkdir -p $${entry}; cd $${entry} && qmake -o Makefile $$PWD/$${entry}/$${entry}.pro && cd ..;
for (entry, PLUGIN_DIRS):plugininfo.commands += make -C $${entry} plugininfo.h;
QMAKE_EXTRA_TARGETS += plugininfo

# Translations:
# make lupdate to update .ts files
lupdate.depends = FORCE plugininfo
for (entry, PLUGIN_DIRS):lupdate.commands += make -C $${entry} lupdate;
QMAKE_EXTRA_TARGETS += lupdate

# make lrelease to build .qm from .ts
lrelease.depends = FORCE
for (entry, PLUGIN_DIRS):lrelease.commands += lrelease $$files($$PWD/$${entry}/translations/*.ts, true);
for (entry, PLUGIN_DIRS):lrelease.commands += rsync -a $$PWD/$${entry}/translations/*.qm $$OUT_PWD/translations/;
QMAKE_EXTRA_TARGETS += lrelease

# For Qt-Creator's code model: Add CPATH to INCLUDEPATH explicitly
INCLUDEPATH += $$(CPATH)

message("Usage: qmake [srcdir] [WITH_PLUGINS=\"...\"] [WITHOUT_PLUGINS=\"...\"]")

isEmpty(WITH_PLUGINS) {
    PLUGINS = $${PLUGIN_DIRS}
} else {
    PLUGINS = $${WITH_PLUGINS}
}
PLUGINS-=$${WITHOUT_PLUGINS}

message("Building plugins:")
for(plugin, PLUGINS) {
    exists($${plugin}) {
        SUBDIRS*= $${plugin}
        message("- $${plugin}")
        # Make sure the libs will be built before the plugins
        equals(plugin, "sunspec") {
            $${plugin}.depends += libnymea-sunspec
        } else {
            $${plugin}.depends += libnymea-modbus
        }
    } else {
        error("Invalid plugin \"$${plugin}\".")
    }
}
