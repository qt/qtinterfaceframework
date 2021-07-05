TEMPLATE = app
TARGET = org-example-echo-qtro-simulation-server
QMAKE_PROJECT_NAME = $$TARGET
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_frontend)

DESTDIR = ..

CONFIG += c++11 ifcodegen
CONFIG -= app_bundle

INCLUDEPATH += $$OUT_PWD/../frontend

QT += qml quick core interfaceframework

IFCODEGEN_TEMPLATE = server_qtro_simulator
IFCODEGEN_SOURCES = ../../org.example.echo.qface
