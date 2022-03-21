set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter5-ipc/imports")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework)

# Interface Framework Generator:
qt_ifcodegen_import_variables(CLUSTER
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(ic_chapter5_imports
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CLUSTER_URI_PATH}"
    URI ${CLUSTER_URI}
    VERSION ${CLUSTER_VERSION}
    PLUGIN_TARGET ic_chapter5_imports
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES
        ${CLUSTER_SOURCES}
)

target_link_libraries(ic_chapter5_imports PUBLIC
    libIc_chapter5
)

install(TARGETS ic_chapter5_imports
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
