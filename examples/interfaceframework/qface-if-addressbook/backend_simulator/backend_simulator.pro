TEMPLATE=lib
TARGET = $$qtLibraryTarget(example_if_addressbook)

QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfAdressBookExample)
DESTDIR = ../interfaceframework
CONFIG += warn_off
INCLUDEPATH += $$OUT_PWD/../frontend
QT += core interfaceframework
CONFIG += ifcodegen plugin

IFCODEGEN_TEMPLATE = backend_simulator
IFCODEGEN_SOURCES = ../example-if-addressbook.qface
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = AddressBookSimulatorPlugin

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-if-addressbook/interfaceframework/
INSTALLS += target

#! [0]
RESOURCES += plugin_resource.qrc
#! [0]

QML_IMPORT_PATH = $$OUT_PWD/qml
