TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_qtro)
DESTDIR = ../interfaceframework

QT += core interfaceframework
CONFIG += ifcodegen plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../instrument-cluster.qface
PLUGIN_TYPE = interfaceframework

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter5-ipc
INSTALLS += target
