TARGET = tst_org-example-echo
QMAKE_PROJECT_NAME = $$TARGET
DESTDIR = ../

QT += testlib core interfaceframework
CONFIG += c++11 ifcodegen testcase

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)

QMAKE_RPATHDIR += $$OUT_PWD/..

IFCODEGEN_TEMPLATE = test
IFCODEGEN_SOURCES = ../../org.example.echo.qface
