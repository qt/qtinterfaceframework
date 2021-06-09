TARGET = chapter5-ipc-server
DESTDIR = ..

QT = core interfaceframework
QT -= gui
CONFIG -= app_bundle
CONFIG += ifcodegen

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend

QFACE_FORMAT = server_qtro_simulator
QFACE_SOURCES = ../instrument-cluster.qface

QML_IMPORT_PATH = $$OUT_PWD/qml

RESOURCES += ../backend_simulator/simulation.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter5-ipc/simulation_server
INSTALLS += target
