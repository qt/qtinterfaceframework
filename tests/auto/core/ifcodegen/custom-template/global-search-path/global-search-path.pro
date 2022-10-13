TARGET = tst_template_global_search_path

QT += testlib
CONFIG += c++11 ifcodegen testcase

QT_IFCODEGEN_TEMPLATE_SEARCH_PATH = $$PWD/../templates

IFCODEGEN_TEMPLATE = custom-test
IFCODEGEN_SOURCES = ../../org.example.echo.qface
