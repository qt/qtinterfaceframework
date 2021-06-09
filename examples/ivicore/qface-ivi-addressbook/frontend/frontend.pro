TARGET = $$qtLibraryTarget(QtIfAdressBookExample)
TEMPLATE = lib
DESTDIR = ..

QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")
QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_EXAMPLE_IF_ADDRESSBOOK_LIB
CONFIG += ifcodegen
QFACE_SOURCES = ../example-if-addressbook.qface
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-ifaddressbook
INSTALLS += target
