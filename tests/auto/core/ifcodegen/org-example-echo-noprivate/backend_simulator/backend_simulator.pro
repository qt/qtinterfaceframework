TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_noprivate_simulator)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)
DESTDIR = ../interfaceframework

CONFIG += warn_off ifcodegen plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = EchoSimulatorPlugin

QT += core interfaceframework

IFCODEGEN_TEMPLATE = backend_simulator
IFCODEGEN_SOURCES = ../../org.example.echo.qface
IFCODEGEN_ANNOTATIONS += ../../no-private.yaml

SOURCES += backend_simulator.cpp

