TARGET = tst_template_full_path

QT += testlib
CONFIG += c++11 ifcodegen testcase

IFCODEGEN_TEMPLATE = $$PWD/../templates/custom-test
IFCODEGEN_SOURCES = ../../org.example.echo.qface
