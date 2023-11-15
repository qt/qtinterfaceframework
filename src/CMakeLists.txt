qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/interfaceframework/configure.cmake")

add_subdirectory(3rdparty)
if(QT_FEATURE_interfaceframework AND QT_FEATURE_ifcodegen)
    add_subdirectory(tools/ifcodegen)
endif()

if(QT_FEATURE_interfaceframework AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(interfaceframework)
    add_subdirectory(helper)
endif()
