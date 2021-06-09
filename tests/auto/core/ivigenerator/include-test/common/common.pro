TEMPLATE=lib
TARGET= $$qtLibraryTarget(include_test_common)
DESTDIR = ../

CONFIG += ifcodegen

QT += interfaceframework interfaceframework-private qml quick

DEFINES += QT_BUILD_COMMON_LIB

macos: QMAKE_SONAME_PREFIX = @rpath

QFACE_SOURCES = "../../qface imports/common.qface"

