TARGET = $$qtLibraryTarget(QtIfRemoteExample)
TEMPLATE = lib
DESTDIR = ..

#! [0]
QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")
#! [0]
QT += interfaceframework qml quick

#! [1]
CONFIG += ifcodegen
IFCODEGEN_SOURCES = ../example-if-remote.qface
#! [1]
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-if-remote
INSTALLS += target
