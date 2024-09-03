cmake_minimum_required(VERSION 3.16)
project(climate LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/climate")


find_package(Qt6 REQUIRED COMPONENTS InterfaceFramework Quick)

qt_standard_project_setup(REQUIRES 6.7)

## Application ##

qt_add_executable(climate WIN32
    main.cpp
)

qt_add_qml_module(climate
    URI Example
    QML_FILES
        Main.qml
        TempControl.qml
        FanSpeedControl.qml
    RESOURCES
        assets/air_minus.png
        assets/air_plus.png
        assets/air_circulation.png
        assets/air_circulation_off.png
        assets/air_circulation_auto.png
)

target_link_libraries(climate PRIVATE
    climate_frontend
    Qt::Quick
)

## Frontend ##

#! [0]
qt_ifcodegen_add_qml_module(climate_frontend
    IDL_FILES example-climate.qface
    TEMPLATE frontend
)
#! [0]

## Backend ##

#! [1]
qt_ifcodegen_add_plugin(climate_backend_simulator
    IDL_FILES example-climate.qface
    TEMPLATE backend_simulator
)
#! [1]

#! [3]
set_target_properties(climate_backend_simulator PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY interfaceframework
)
#! [3]

#! [2]
target_link_libraries(climate_backend_simulator PRIVATE
    climate_frontend
)
#! [2]

set_property(TARGET climate APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

install(TARGETS climate climate_frontend
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
install(TARGETS climate_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)

if (TARGET climate_frontendplugin)
    set_target_properties(climate_frontendplugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS climate_frontendplugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/ClimateModule"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/ClimateModule"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/ClimateModule"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/ClimateModule/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/ClimateModule"
)
