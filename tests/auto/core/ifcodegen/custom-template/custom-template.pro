TARGET = tst_custom-template

QT += testlib
CONFIG += c++11 ifcodegen testcase

QFACE_FORMAT = $$PWD/custom-test
QFACE_SOURCES = ../org.example.echo.qface
