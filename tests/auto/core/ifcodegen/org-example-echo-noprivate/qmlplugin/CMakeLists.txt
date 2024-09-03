set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt_ifcodegen_import_variables(ECHO_NOPRIVATE
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE qmlplugin
    ANNOTATION_FILES ../../no-private.yaml
)

qt_add_qml_module(echo_noprivate_qmlplugin
    URI ${ECHO_NOPRIVATE_URI}
    VERSION ${ECHO_NOPRIVATE_VERSION}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/echo_noprivate_qmlplugin/"
    RESOURCE_PREFIX "/"
    PLUGIN_TARGET echo_noprivate_qmlplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${ECHO_NOPRIVATE_SOURCES}
)

target_link_libraries(echo_noprivate_qmlplugin PRIVATE
    echo_noprivate_frontend
)
