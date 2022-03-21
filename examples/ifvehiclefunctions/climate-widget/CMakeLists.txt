cmake_minimum_required(VERSION 3.16)
project(climate_widget LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/ifvehiclefunctions/climate_widget")

find_package(Qt6 REQUIRED COMPONENTS Core Gui IfVehicleFunctions InterfaceFramework Widgets)

qt_add_executable(climate_widget
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
)

set_target_properties(climate_widget PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(climate_widget PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::InterfaceFramework
    Qt::IfVehicleFunctions
)

install(TARGETS climate_widget
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
