set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter7-own-template")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework)

qt_add_plugin(ic_chapter7_simulation PLUGIN_TYPE interfaceframework)
set_target_properties(ic_chapter7_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(ic_chapter7_simulation
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE backend_simulator
)

target_link_libraries(ic_chapter7_simulation PUBLIC
    libIc_chapter7
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/backend_simulator/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

# Resources:
set(simulation_resource_files
    "simulation.qml"
)

qt_add_resources(ic_chapter7_simulation "simulation"
    PREFIX
        "/"
    FILES
        ${simulation_resource_files}
)

install(TARGETS ic_chapter7_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
