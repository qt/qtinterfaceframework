TEMPLATE = lib
QT = interfaceframework
CONFIG += c++11 plugin

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noprivate_frontend)

IFCODEGEN_TEMPLATE = qmlplugin
IFCODEGEN_SOURCES = ../../org.example.echo.qface
IFCODEGEN_ANNOTATIONS += ../../no-private.yaml

load(ifcodegen)
