set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-addressbook/interfaceframework/")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework)

qt_add_plugin(addressbook_backend_simulator PLUGIN_TYPE interfaceframework)
set_target_properties(addressbook_backend_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(addressbook_backend_simulator
    IDL_FILES ../example-addressbook.qface
    TEMPLATE backend_simulator
)

target_link_libraries(addressbook_backend_simulator PUBLIC
    QtIfAdressBookExample
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/backend_simulator/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

# Resources:
#! [0]
set(plugin_resource_resource_files
    "simulation.qml"
)

qt_add_resources(addressbook_backend_simulator "plugin_resource"
    PREFIX
        "/plugin_resource"
    FILES
        ${plugin_resource_resource_files}
)

#! [0]

install(TARGETS addressbook_backend_simulator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
