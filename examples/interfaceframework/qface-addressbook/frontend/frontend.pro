TARGET = $$qtLibraryTarget(QtIfAdressBookExample)
TEMPLATE = lib
DESTDIR = ..

QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")
QT += interfaceframework qml quick

CONFIG += ifcodegen
IFCODEGEN_SOURCES = ../example-addressbook.qface
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-addressbook
INSTALLS += target
