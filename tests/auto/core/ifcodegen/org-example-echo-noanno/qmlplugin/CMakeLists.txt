set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen_import_variables(ECHO_NOANNO
    IDL_FILES ../../org.example.echo.noannotation.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(echo_noanno_qmlplugin
    URI ${ECHO_NOANNO_URI}
    VERSION ${ECHO_NOANNO_VERSION}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/echo_noanno_qmlplugin/"
    RESOURCE_PREFIX "/"
    PLUGIN_TARGET echo_noanno_qmlplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${ECHO_NOANNO_SOURCES}
)

target_link_libraries(echo_noanno_qmlplugin PRIVATE
    echo_noanno_frontend
)
