TEMPLATE=lib
TARGET= $$qtLibraryTarget(echo_noprivate_frontend)
DESTDIR = ../

CONFIG += ifcodegen

QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_ORG_EXAMPLE_ECHO_LIB

macos: QMAKE_SONAME_PREFIX = @rpath

IFCODEGEN_SOURCES = ../../org.example.echo.qface
IFCODEGEN_ANNOTATIONS += ../../no-private.yaml

