TEMPLATE = app

QT += qml quick widgets ivicore

CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# install
target.path = $$[QT_INSTALL_EXAMPLES]/media/mediaplayer
INSTALLS += target
