set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/ch3-simulation-backend/imports")

find_package(Qt6 REQUIRED COMPONENTS Core Gui InterfaceFramework)

# Interface Framework Generator:
qt_ifcodegen_import_variables(CLUSTER
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(ic_ch3_imports
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CLUSTER_URI_PATH}"
    URI ${CLUSTER_URI}
    VERSION ${CLUSTER_VERSION}
    CLASS_NAME InstrumentClusterModuleQmlPlugin
    RESOURCE_PREFIX "/"
    PLUGIN_TARGET ic_ch3_imports
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    IMPORTS QtInterfaceFramework
    SOURCES
        ${CLUSTER_SOURCES}
)

target_link_libraries(ic_ch3_imports PUBLIC
    libIc_ch3
)

install(TARGETS ic_ch3_imports
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
