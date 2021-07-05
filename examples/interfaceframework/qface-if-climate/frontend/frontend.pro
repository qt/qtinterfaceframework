TARGET = $$qtLibraryTarget(QtIfClimateExample)
TEMPLATE = lib
DESTDIR = ..

#! [0]
QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")
#! [0]
QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_EXAMPLE_IF_CLIMATE_LIB
#! [1]
CONFIG += ifcodegen
IFCODEGEN_SOURCES = ../example-if-climate.qface
#! [1]
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-if-climate
INSTALLS += target
