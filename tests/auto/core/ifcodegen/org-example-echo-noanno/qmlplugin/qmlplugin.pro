TEMPLATE = lib
QT = interfaceframework ifvehiclefunctions
CONFIG += c++11 plugin

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_noannotation_frontend)

IFCODEGEN_TEMPLATE = qmlplugin
IFCODEGEN_SOURCES = ../../org.example.echo.noannotation.qface

load(ifcodegen)
