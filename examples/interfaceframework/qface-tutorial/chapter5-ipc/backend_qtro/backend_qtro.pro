TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_qtro)
DESTDIR = ../interfaceframework

QT += core interfaceframework
CONFIG += ifcodegen plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

IFCODEGEN_TEMPLATE = backend_qtro
IFCODEGEN_SOURCES = ../instrument-cluster.qface
PLUGIN_TYPE = interfaceframework

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter5-ipc
INSTALLS += target
