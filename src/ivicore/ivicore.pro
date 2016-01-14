TARGET = QtIVICore

QT = core core-private qml
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtiviservicemanager.h \
    qtiviserviceinterface.h \
    qtiviservicemanager_p.h \
    qtiviserviceobject.h \
    qtiviabstractfeature.h \
    qtiviglobal.h \
    qtiviproxyserviceobject_p.h

SOURCES += \
    qtiviservicemanager.cpp \
    qtiviserviceobject.cpp \
    qtiviabstractfeature.cpp \
    qtiviproxyserviceobject_p.cpp \

load(qt_module)
