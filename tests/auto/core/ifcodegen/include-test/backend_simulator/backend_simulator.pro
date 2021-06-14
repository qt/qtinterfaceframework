TEMPLATE=lib
TARGET = $$qtLibraryTarget(include_test_simulator)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)
DESTDIR = ../interfaceframework

CONFIG += warn_off ifcodegen plugin

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = IncludeTestSimulatorPlugin

QT += core interfaceframework

QFACE_FORMAT = backend_simulator
QFACE_SOURCES = ../../include-test.qface
QFACE_IMPORT_PATH += "../../qface imports"
