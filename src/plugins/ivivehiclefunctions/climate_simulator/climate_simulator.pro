TARGET = climate_simulator

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QtIVIClimateControlBackendInterface

QT       += core ivicore ivivehiclefunctions

load(qt_plugin)


SOURCES += climateplugin.cpp \
    climatecontrolbackend.cpp

HEADERS += climateplugin.h \
    climatecontrolbackend.h

DISTFILES += climate_simulator.json
