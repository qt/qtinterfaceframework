QT       += core gui interfaceframework ifvehiclefunctions
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = climate_widget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/ifvehiclefunctions/climate_widget
INSTALLS += target
