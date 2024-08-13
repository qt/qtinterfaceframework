set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_ifcodegen_add_plugin(ic_chapter7_dbus
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE ../templates/backend_dbus
)

set_target_properties(ic_chapter7_dbus PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_include_directories(ic_chapter7_dbus PRIVATE
    ../demo_server
)

target_link_libraries(ic_chapter7_dbus PUBLIC
    libIc_chapter7
    Qt::Core
    Qt::DBus
    Qt::Gui
    Qt::InterfaceFramework
)

install(TARGETS ic_chapter7_dbus
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
