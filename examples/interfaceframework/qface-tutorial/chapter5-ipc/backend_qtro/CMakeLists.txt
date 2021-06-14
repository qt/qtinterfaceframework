# Generated from backend_qtro.pro.
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

qt_add_plugin(instrumentcluster_qtro)
set_target_properties(instrumentcluster_qtro PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen(instrumentcluster_qtro
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT backend_qtro
)

target_link_libraries(instrumentcluster_qtro PUBLIC
    QtIfInstrumentCluster_Chapter5
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS instrumentcluster_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
