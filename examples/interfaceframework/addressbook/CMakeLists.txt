cmake_minimum_required(VERSION 3.16)
project(if_addressbook LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/if_addressbook")


find_package(Qt6 REQUIRED COMPONENTS InterfaceFramework Quick)

qt_standard_project_setup(REQUIRES 6.7)

## Application ##

qt_add_executable(if_addressbook WIN32
    main.cpp
)

qt_add_qml_module(if_addressbook
    URI Example
    QML_FILES Main.qml
)

target_link_libraries(if_addressbook PRIVATE
    if_addressbook_frontend
    Qt::Quick
)

## Frontend ##

qt_ifcodegen_add_qml_module(if_addressbook_frontend
    IDL_FILES example-addressbook.qface
    TEMPLATE frontend
)

## Backend ##

qt_ifcodegen_add_plugin(if_addressbook_backend_simulator
    IDL_FILES example-addressbook.qface
    TEMPLATE backend_simulator
)

set_target_properties(if_addressbook_backend_simulator PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY interfaceframework
)

target_link_libraries(if_addressbook_backend_simulator PRIVATE
    if_addressbook_frontend
)

#! [backend_simulation_resource]
qt_add_resources(if_addressbook_backend_simulator "plugin_resource"
    PREFIX
        "/plugin_resource"
    FILES
         "simulation.qml"
)
#! [backend_simulation_resource]

set_property(TARGET if_addressbook APPEND PROPERTY QT_ANDROID_EXTRA_PLUGINS
    "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

install(TARGETS if_addressbook if_addressbook_frontend
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
install(TARGETS if_addressbook_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)

if (TARGET if_addressbook_frontendplugin)
    set_target_properties(if_addressbook_frontendplugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS if_addressbook_frontendplugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/AddressBookModule"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/AddressBookModule"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/AddressBookModule"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/AddressBookModule/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/AddressBookModule"
)
