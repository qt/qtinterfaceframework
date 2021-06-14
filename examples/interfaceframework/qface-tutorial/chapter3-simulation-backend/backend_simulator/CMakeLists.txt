# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter3-simulation-backend")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)

qt_add_plugin(instrumentcluster_chapter3_simulation)
set_target_properties(instrumentcluster_chapter3_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen(instrumentcluster_chapter3_simulation
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_simulator
)

target_link_libraries(instrumentcluster_chapter3_simulation PUBLIC
    QtIfInstrumentCluster_Chapter3
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS instrumentcluster_chapter3_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
