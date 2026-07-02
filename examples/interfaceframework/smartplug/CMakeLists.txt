cmake_minimum_required(VERSION 3.16)
project(smartplug LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/smartplug")

find_package(Qt6 REQUIRED COMPONENTS InterfaceFramework Quick Mqtt)

qt_standard_project_setup(REQUIRES 6.7)

## Application ##

qt_add_executable(smartplug WIN32 MACOSX_BUNDLE
    main.cpp
)

qt_add_qml_module(smartplug
    URI Example.SmartPlug
    QML_FILES
        Main.qml
        BrokerBar.qml
    NO_LINT
)

target_link_libraries(smartplug PRIVATE
    smartplug_frontend
    Qt::Quick
)

## Frontend ##

qt_ifcodegen_add_qml_module(smartplug_frontend
    IDL_FILES example-smartplug.qface
    TEMPLATE frontend
)

## MQTT Backend ##

qt_ifcodegen_add_plugin(smartplug_backend_mqtt
    IDL_FILES example-smartplug.qface
    TEMPLATE backend_mqtt
)

set_target_properties(smartplug_backend_mqtt PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY interfaceframework
)

target_link_libraries(smartplug_backend_mqtt PRIVATE
    smartplug_frontend
)

set_property(TARGET smartplug APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

## Simulation device ##
# A small helper that plays the role of a real smart plug on the broker, so the
# example works out-of-the-box against a local broker without extra hardware.

qt_add_executable(smartplug-simulation
    smartplug-simulation/main.cpp
)

target_link_libraries(smartplug-simulation PRIVATE
    Qt::Core
    Qt::Mqtt
)

install(TARGETS smartplug smartplug_frontend smartplug-simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
install(TARGETS smartplug_backend_mqtt
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
