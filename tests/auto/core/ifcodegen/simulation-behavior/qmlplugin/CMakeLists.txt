qt_ifcodegen_import_variables(
    SIMU
    IDL_FILES ../simu.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(
    simuplugin
    OUTPUT_DIRECTORY "../imports/${SIMU_URI_PATH}"
    URI ${SIMU_URI}
    VERSION ${SIMU_VERSION}
    RESOURCE_PREFIX "/"
    PLUGIN_TARGET simuplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    SOURCES ${SIMU_SOURCES}
)

target_link_libraries(
    simuplugin
    PRIVATE Qt::InterfaceFramework simu
)
