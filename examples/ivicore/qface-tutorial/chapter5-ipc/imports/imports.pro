TEMPLATE = lib
CONFIG += plugin
QT += interfaceframework

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend

QFACE_FORMAT = qmlplugin
QFACE_SOURCES = ../instrument-cluster.qface

load(ifcodegen)

DESTDIR = $$OUT_PWD/$$replace(URI, \\., /)
QMAKE_RPATHDIR += $$QMAKE_REL_RPATH_BASE/../../../../

exists($$OUT_PWD/qmldir) {
    cpqmldir.files = $$OUT_PWD/qmldir \
                     $$OUT_PWD/plugins.qmltypes
    cpqmldir.path = $$DESTDIR
    cpqmldir.CONFIG = no_check_exist
    COPIES += cpqmldir
}

target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter5-ipc/imports
INSTALLS += target
