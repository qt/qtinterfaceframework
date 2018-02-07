TARGET = $$qtLibraryTarget(QtIviClimateExample)
TEMPLATE = lib
CONFIG += ivigenerator
DESTDIR = ..
macos: CONFIG += debug_and_release build_all

QT_FOR_CONFIG += ivicore
!qtConfig(ivigenerator): error("No ivigenerator available")

QT += ivicore ivicore-private qml quick

DEFINES += QT_BUILD_CLIMATE_LIB

QFACE_SOURCES = ../example-ivi-climate.qface

target.path = $$[QT_INSTALL_EXAMPLES]/ivicore/qface-ivi-climate/frontend
INSTALLS += target
