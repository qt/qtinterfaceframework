# Generated from imports.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter3-simulation-backend/imports")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)


# Interface Framework Generator:
qt6_ifcodegen(instrument_cluster_chapter3_imports # special case
    QFACE_SOURCES ../instrument-cluster.qface
    QFACE_FORMAT qmlplugin
)

target_link_libraries(instrument_cluster_chapter3_imports PUBLIC # special case
    QtIfInstrumentCluster_Chapter3
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS instrument_cluster_chapter3_imports # special case
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
