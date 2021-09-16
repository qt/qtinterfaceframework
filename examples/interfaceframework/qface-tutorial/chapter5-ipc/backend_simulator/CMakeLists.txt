# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter5-ipc")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_plugin(ic_chapter5_simulation)
set_target_properties(ic_chapter5_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(ic_chapter5_simulation
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE backend_simulator
)

target_link_libraries(ic_chapter5_simulation PUBLIC
    libIc_chapter5
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)


# Resources:
set(simulation_resource_files
    "simulation.qml"
)

qt_add_resources(ic_chapter5_simulation "simulation"
    PREFIX
        "/"
    FILES
        ${simulation_resource_files}
)

install(TARGETS ic_chapter5_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
