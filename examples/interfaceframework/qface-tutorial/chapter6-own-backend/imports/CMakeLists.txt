# Generated from imports.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter6-own-backend/imports")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS InterfaceFramework)


# Interface Framework Generator:
qt6_ifcodegen_import_variables(CLUSTER
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(instrument_cluster_chapter6_imports
    URI ${CLUSTER_URI}
    VERSION ${CLUSTER_VERSION}
    PLUGIN_TARGET instrument_cluster_chapter6_imports
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${CLUSTER_SOURCES}
)

target_link_libraries(instrument_cluster_chapter6_imports PUBLIC
    QtIfInstrumentCluster_Chapter6
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS instrument_cluster_chapter6_imports
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
