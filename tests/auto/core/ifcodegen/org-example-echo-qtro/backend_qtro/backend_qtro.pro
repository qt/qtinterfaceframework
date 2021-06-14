TEMPLATE=lib
TARGET = $$qtLibraryTarget(echo_backend_qtro)
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)
DESTDIR = ../interfaceframework

CONFIG += warn_off ifcodegen plugin

INCLUDEPATH += $$OUT_PWD/../frontend
PLUGIN_TYPE = interfaceframework
PLUGIN_CLASS_NAME = EchoQtROPlugin

QT += core interfaceframework

macos: QMAKE_SONAME_PREFIX = @rpath

QFACE_FORMAT = backend_qtro
QFACE_SOURCES = ../../org.example.echo.qface
