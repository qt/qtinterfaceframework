set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter7-own-template")

find_package(Qt6 REQUIRED COMPONENTS Core DBus Gui InterfaceFramework)

qt_add_plugin(ic_chapter7_dbus)
set_target_properties(ic_chapter7_dbus PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(ic_chapter7_dbus
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE ../templates/backend_dbus
)

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
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
