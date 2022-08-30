#####################################################################
## tst_servicemanagertest Test:
#####################################################################

qt_internal_add_test(tst_servicemanagertest
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    SOURCES
        tst_servicemanagertest.cpp
    DEFINES
        SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/\\\"
    LIBRARIES
        simpleStaticPlugin
        wrongmetadataStaticPlugin
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
)

# This dummy QML file is not used by the test itself, but helps
# to let tools like androiddeployqt know what QML dependencies we
# have and package everything correctly
qt_add_resources(tst_servicemanagertest "qml"
    PREFIX "/"
    FILES qml_dependencies.qml
)

qt_internal_extend_target(tst_servicemanagertest CONDITION QT_FEATURE_debug_and_release
    DEFINES
        DEBUG_AND_RELEASE
)
add_subdirectory(simple_plugin)
add_subdirectory(simple_plugin_static)
add_subdirectory(wrong_plugin)
add_subdirectory(wrongmetadata_plugin)
add_subdirectory(wrongmetadata_plugin_static)

# QT_ANDROID_EXTRA_PLUGINS requires a list of directories, not files!
set_target_properties(tst_servicemanagertest PROPERTIES
    QT_ANDROID_EXTRA_PLUGINS "${CMAKE_CURRENT_BINARY_DIR}/interfaceframework"
)

