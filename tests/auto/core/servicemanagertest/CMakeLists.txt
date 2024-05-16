#####################################################################
## tst_servicemanagertest Test:
#####################################################################

qt_internal_add_test(tst_servicemanagertest
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    SOURCES
        tst_servicemanagertest.cpp
    LIBRARIES
        simpleStaticPlugin
        wrongmetadataStaticPlugin
        Qt::InterfaceFramework
        Qt::InterfaceFrameworkPrivate
)

qt_add_resources(tst_servicemanagertest "qml"
    PREFIX "/"
    FILES testdata/loadserviceobject.qml
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
