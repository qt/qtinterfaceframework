TARGET = chapter7-demo-server
DESTDIR = ..

QT += interfaceframework dbus
QT -= gui
CONFIG -= app_bundle

LIBS += -L$$OUT_PWD/../ -l$$qtLibraryTarget(QtIfInstrumentCluster)
INCLUDEPATH += $$OUT_PWD/../frontend

cluster.files = instrumentcluster.xml
cluster.header_flags += -i dbus_conversion.h

DBUS_ADAPTORS += cluster

CONFIG += install_ok  # Do not cargo-cult this!
target.path = $$[QT_INSTALL_EXAMPLES]/interfaceframework/qface-tutorial/chapter7-own-template
INSTALLS += target

SOURCES += \
    instrumentcluster.cpp \
    main.cpp

HEADERS += \
    instrumentcluster.h \
    dbus_conversion.h \
