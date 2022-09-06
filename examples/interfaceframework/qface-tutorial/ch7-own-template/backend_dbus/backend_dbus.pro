TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_dbus)
DESTDIR = ../interfaceframework

QT += interfaceframework dbus
CONFIG += plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend $$PWD/../demo_server
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

PLUGIN_TYPE = interfaceframework

IFCODEGEN_TEMPLATE = $$PWD/../templates/backend_dbus
IFCODEGEN_SOURCES = ../instrument-cluster.qface

HEADERS += \
    ../demo_server/dbus_conversion.h \

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter7-own-template
INSTALLS += target
