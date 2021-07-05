TARGET = $$qtLibraryTarget(QtIfInstrumentCluster)
TEMPLATE = lib
DESTDIR = ..

QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_EXAMPLE_IF_INSTRUMENTCLUSTER_LIB
CONFIG += ifcodegen
IFCODEGEN_SOURCES = ../instrument-cluster.qface

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter6-own-backend
INSTALLS += target
