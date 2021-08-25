TARGET = tst_template_extra_search_path

QT += testlib
CONFIG += c++11 ifcodegen testcase

IFCODEGEN_EXTRA_TEMPLATE_SEARCH_PATH = $$PWD/../
IFCODEGEN_TEMPLATE = custom-test
IFCODEGEN_SOURCES = ../../org.example.echo.qface
