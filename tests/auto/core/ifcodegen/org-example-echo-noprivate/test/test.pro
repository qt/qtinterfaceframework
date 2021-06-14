TARGET = tst_org-example-echo-noprivate
QMAKE_PROJECT_NAME = $$TARGET
DESTDIR = ../

QT += testlib core interfaceframework
CONFIG += c++11 ifcodegen testcase

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)

QMAKE_RPATHDIR += $$OUT_PWD/..

QFACE_FORMAT = test
QFACE_SOURCES = ../../org.example.echo.qface
QFACE_ANNOTATIONS += ../../no-private.yaml
