# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen_import_variables(ECHO
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE qmlplugin
)

qt_add_qml_module(echo_qmlplugin
    URI ${ECHO_URI}
    VERSION ${ECHO_VERSION}
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/echo_qmlplugin/"
    PLUGIN_TARGET echo_qmlplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${ECHO_SOURCES}
)

target_link_libraries(echo_qmlplugin PRIVATE
    echo_frontend
    Qt::InterfaceFramework
)
