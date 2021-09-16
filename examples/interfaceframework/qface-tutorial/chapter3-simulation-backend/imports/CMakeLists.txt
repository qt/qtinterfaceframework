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
qt_ifcodegen_import_variables(CLUSTER
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(ic_chapter3_imports
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CLUSTER_URI_PATH}"
    URI ${CLUSTER_URI}
    VERSION ${CLUSTER_VERSION}
    PLUGIN_TARGET ic_chapter3_imports
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        ${CLUSTER_SOURCES}
)

target_link_libraries(ic_chapter3_imports PUBLIC # special case
    libIc_chapter3
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS ic_chapter3_imports # special case
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
