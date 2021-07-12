TARGET = $$qtLibraryTarget(QtIfInstrumentCluster)
TEMPLATE = lib
DESTDIR = ..

QT += interfaceframework qml quick

CONFIG += ifcodegen
IFCODEGEN_SOURCES = ../instrument-cluster.qface

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter3-simulation-backend
INSTALLS += target
