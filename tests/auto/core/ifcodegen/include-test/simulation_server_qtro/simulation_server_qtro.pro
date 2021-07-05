TEMPLATE = app
TARGET = org-example-echo-qtro-simulation-server
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)

DESTDIR = ..

CONFIG += c++11 ifcodegen
CONFIG -= app_bundle

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common

QT += qml quick core interfaceframework

IFCODEGEN_TEMPLATE = server_qtro_simulator
IFCODEGEN_SOURCES = ../../include-test.qface
IFCODEGEN_IMPORT_PATH += "../../qface imports"
