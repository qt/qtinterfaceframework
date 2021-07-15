# Generated from src.pro.

qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/interfaceframework/configure.cmake") # special case
qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/ifvehiclefunctions/configure.cmake") # special case
qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/ifmedia/configure.cmake") # special case

add_subdirectory(3rdparty) # special case
if(QT_FEATURE_interfaceframework AND QT_FEATURE_ifcodegen)
    add_subdirectory(tools/ifcodegen)
endif()

if(QT_FEATURE_interfaceframework AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(interfaceframework)
    add_subdirectory(helper)
endif()
if(QT_FEATURE_interfaceframework AND QT_FEATURE_ifcodegen AND QT_FEATURE_ifvehiclefunctions AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(ifvehiclefunctions)
endif()
if(QT_FEATURE_interfaceframework AND QT_FEATURE_ifmedia AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(ifmedia)
endif()
# special case begin
if(QT_FEATURE_interfaceframework AND QT_FEATURE_remoteobjects AND QT_FEATURE_vehiclefunctions_qtro_simulation_server AND NOT ANDROID AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(tools/vehiclefunctions-simulation-server)
endif()
if(QT_FEATURE_interfaceframework AND QT_FEATURE_media_qtro_simulation_server AND QT_FEATURE_remoteobjects AND NOT ANDROID AND NOT QT_FEATURE_host_tools_only)
    add_subdirectory(tools/media-simulation-server)
endif()
add_subdirectory(plugins)
# special case end

