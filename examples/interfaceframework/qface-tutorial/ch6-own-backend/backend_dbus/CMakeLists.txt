set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_plugin(ic_chapter6_dbus PLUGIN_TYPE interfaceframework)
set_target_properties(ic_chapter6_dbus PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

set_source_files_properties(../demo_server/instrumentcluster.xml PROPERTIES INCLUDE dbus_conversion.h)
set(cluster_SRCS)
qt_add_dbus_interface(cluster_SRCS
    ../demo_server/instrumentcluster.xml
    instrumentcluster_interface
)

target_sources(ic_chapter6_dbus PRIVATE
    ../demo_server/dbus_conversion.h
    instrumentclusterbackend.cpp instrumentclusterbackend.h
    instrumentclusterplugin.cpp instrumentclusterplugin.h
    ${cluster_SRCS}
)

target_include_directories(ic_chapter6_dbus PRIVATE
    ../demo_server
)

target_link_libraries(ic_chapter6_dbus PUBLIC
    libIc_chapter6
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS ic_chapter6_dbus
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
