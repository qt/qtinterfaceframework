TEMPLATE=lib
TARGET= $$qtLibraryTarget(echo_noannotation_frontend)
DESTDIR = ../

CONFIG += ifcodegen

QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_ORG_EXAMPLE_ECHO_LIB

macos: QMAKE_SONAME_PREFIX = @rpath

IFCODEGEN_SOURCES = ../../org.example.echo.noannotation.qface

