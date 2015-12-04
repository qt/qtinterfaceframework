TARGET = QtIVICore

QT = core core-private qml
CONFIG += c++11
VERSION = 1.0.0

QMAKE_DOCS = $$PWD/doc/qtivicore.qdocconf
OTHER_FILES += $$PWD/doc/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h \
    qtiviserviceinterface.h \
    qtiviservicemanager_p.h \
    qtiviserviceobject.h \
    qtiviabstractfeature.h \
    qtiviglobal.h \
    qtiviproxyserviceobject_p.h \

SOURCES += \
    qtiviservicemanager.cpp \
    qtiviserviceobject.cpp \
    qtiviabstractfeature.cpp \
    qtiviproxyserviceobject_p.cpp \

load(qt_module)
