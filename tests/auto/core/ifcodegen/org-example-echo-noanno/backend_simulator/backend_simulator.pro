TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_noannotation_simulator)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noannotation_frontend)
DESTDIR = ../interfaceframework

CONFIG += warn_off ifcodegen plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = EchoSimulatorPlugin

QT += core interfaceframework

IFCODEGEN_TEMPLATE = backend_simulator
IFCODEGEN_SOURCES = ../../org.example.echo.noannotation.qface
