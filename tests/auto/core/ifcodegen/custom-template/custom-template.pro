TARGET = tst_custom-template

QT += testlib
CONFIG += c++11 ifcodegen testcase

IFCODEGEN_TEMPLATE = $$PWD/custom-test
IFCODEGEN_SOURCES = ../org.example.echo.qface
