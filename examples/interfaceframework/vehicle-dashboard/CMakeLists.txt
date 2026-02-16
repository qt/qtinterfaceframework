cmake_minimum_required(VERSION 3.16)
project(vehicle-dashboard LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/vehicle-dashboard")


find_package(Qt6 REQUIRED COMPONENTS InterfaceFramework Quick)

qt_standard_project_setup(REQUIRES 6.7)

## Application ##

qt_add_executable(vehicle-dashboard WIN32
    main.cpp
)

qt_add_qml_module(vehicle-dashboard
    URI Example
    QML_FILES
        Main.qml
        ClimatePanel.qml
        VehiclePanel.qml
        ZoneClimateColumn.qml
)

target_link_libraries(vehicle-dashboard PRIVATE
    vehicle-dashboard_frontend
    Qt::Quick
)

## Frontend ##

#! [0]
qt_ifcodegen_add_qml_module(vehicle-dashboard_frontend
    IDL_FILES example-vehicledashboard.qface
    TEMPLATE frontend
)
#! [0]

## Backend ##

#! [1]
qt_ifcodegen_add_plugin(vehicle-dashboard_backend_simulator
    IDL_FILES example-vehicledashboard.qface
    TEMPLATE backend_simulator
)
#! [1]

qt_add_resources(vehicle-dashboard_backend_simulator "custom_panel"
    PREFIX "/simulation"
    FILES
        TirePressureControl.qml
        VehicleDashboardCustomPanel.qml
)

#! [3]
set_target_properties(vehicle-dashboard_backend_simulator PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY interfaceframework
)
#! [3]

#! [2]
target_link_libraries(vehicle-dashboard_backend_simulator PRIVATE
    vehicle-dashboard_frontend
)
#! [2]

set_property(TARGET vehicle-dashboard APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

install(TARGETS vehicle-dashboard vehicle-dashboard_frontend
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
install(TARGETS vehicle-dashboard_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)

if (TARGET vehicle-dashboard_frontendplugin)
    set_target_properties(vehicle-dashboard_frontendplugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS vehicle-dashboard_frontendplugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/VehicleDashboard"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/VehicleDashboard"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/VehicleDashboard"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/VehicleDashboard/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/VehicleDashboard"
)
