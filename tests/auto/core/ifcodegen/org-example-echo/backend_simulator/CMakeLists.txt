# Generated from backend_simulator.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_simulator)
set_target_properties(echo_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)
target_sources(echo_simulator PRIVATE
    backend_simulator.cpp
)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_simulator
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_simulator
)

target_link_libraries(echo_simulator PUBLIC
    echo_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
