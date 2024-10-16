TARGET = tst_include
QMAKE_PROJECT_NAME = $$TARGET
DESTDIR = ../

QT += testlib core interfaceframework
CONFIG += c++11 ifcodegen testcase

INCLUDEPATH += $$OUT_PWD/../frontend $$OUT_PWD/../common
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(include_test_frontend) -l$$qtLibraryTarget(include_test_common)

QMAKE_RPATHDIR += $$OUT_PWD/..

IFCODEGEN_TEMPLATE = test
IFCODEGEN_SOURCES = ../../include-test.qface
IFCODEGEN_IMPORT_PATH += "../../qface imports"
