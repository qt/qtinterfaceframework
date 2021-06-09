# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter4-simulation-behavior")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_plugin(instrumentcluster_chapter4_simulation)
set_target_properties(instrumentcluster_chapter4_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen(instrumentcluster_chapter4_simulation
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_simulator
)

target_link_libraries(instrumentcluster_chapter4_simulation PUBLIC
    QtIfInstrumentCluster_Chapter4
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)


# Resources:
set(simulation_resource_files
    "simulation.qml"
)

qt6_add_resources(instrumentcluster_chapter4_simulation "simulation"
    PREFIX
        "/"
    FILES
        ${simulation_resource_files}
)

install(TARGETS instrumentcluster_chapter4_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
