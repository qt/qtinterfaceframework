TEMPLATE = lib
TARGET = $$qtLibraryTarget(instrumentcluster_simulation)
DESTDIR = ../interfaceframework

QT += core interfaceframework
CONFIG += ifcodegen plugin

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../instrument-cluster.qface
PLUGIN_TYPE = interfaceframework

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$OUT_PWD/../frontend/qml

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter3-simulation-backend
INSTALLS += target
