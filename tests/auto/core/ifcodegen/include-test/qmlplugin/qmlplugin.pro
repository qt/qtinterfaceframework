TEMPLATE = lib
QT = interfaceframework
CONFIG += c++11 plugin

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)

IFCODEGEN_TEMPLATE = qmlplugin
IFCODEGEN_SOURCES = ../../include-test.qface
IFCODEGEN_IMPORT_PATH += "../../qface imports"

load(ifcodegen)
