# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noprivate_simulator)
set_target_properties(echo_noprivate_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen(echo_noprivate_simulator
    IFCODEGEN_SOURCES ../../org.example.echo.qface
    IFCODEGEN_TEMPLATE backend_simulator
    IFCODEGEN_ANNOTATIONS ../../no-private.yaml
)

target_link_libraries(echo_noprivate_simulator PUBLIC
    echo_noprivate_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
