TARGET = tst_legacy_template

QT += testlib
CONFIG += c++11 ifcodegen testcase

IFCODEGEN_TEMPLATE = $$PWD/../templates/legacy-custom-test
IFCODEGEN_SOURCES = ../../org.example.echo.qface
