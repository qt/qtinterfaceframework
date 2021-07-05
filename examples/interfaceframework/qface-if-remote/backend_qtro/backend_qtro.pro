TEMPLATE=lib
TARGET = $$qtLibraryTarget(example_if_remote)

QT_FOR_CONFIG += interfaceframework
!qtConfig(ifcodegen): error("No ifcodegen available")

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfRemoteExample)
#! [0]
DESTDIR = ../interfaceframework
#! [0]
CONFIG += warn_off
#! [1]
INCLUDEPATH += $$OUT_PWD/../frontend
#! [1]
QT += core interfaceframework
#! [2]
CONFIG += ifcodegen plugin
IFCODEGEN_TEMPLATE = backend_qtro
IFCODEGEN_SOURCES = ../example-if-remote.qface
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = RemoteClientQtROPlugin
#! [2]
CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-if-remote/interfaceframework/
INSTALLS += target
