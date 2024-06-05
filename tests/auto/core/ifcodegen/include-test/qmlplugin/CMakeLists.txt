set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen_import_variables(INCLUDE_TEST
    IDL_FILES ../../include-test.qface
    TEMPLATE qmlplugin
    IDL_IMPORT_PATH ../../qface\ imports
)

qt_add_qml_module(include_test_qmlplugin
    URI ${INCLUDE_TEST_URI}
    VERSION ${INCLUDE_TEST_VERSION}
    RESOURCE_PREFIX "/"
    PLUGIN_TARGET include_test_qmlplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${INCLUDE_TEST_SOURCES}
)

target_link_libraries(include_test_qmlplugin PRIVATE
    include_test_frontend
)
