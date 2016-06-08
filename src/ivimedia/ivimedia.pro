TARGET = QtIviMedia

QT = core-private ivicore ivicore-private
CONFIG += c++11
VERSION = 1.0.0

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

CMAKE_MODULE_TESTS = '-'

HEADERS += \
    qtivimediaglobal.h \
    qivimediaplayer.h \
    qivimediaplayer_p.h \
    qiviplayableitem.h \
    qivimediaplayerbackendinterface.h \
    qiviplayqueue.h \
    qiviplayqueue_p.h

SOURCES += \
    qivimediaplayer.cpp \
    qiviplayableitem.cpp \
    qivimediaplayerbackendinterface.cpp \
    qiviplayqueue.cpp

load(qt_module)
