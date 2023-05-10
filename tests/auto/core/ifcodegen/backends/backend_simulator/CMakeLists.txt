set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_qtro_simulator)
set_target_properties(echo_qtro_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_qtro_simulator
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_simulator
)

target_link_libraries(echo_qtro_simulator PUBLIC
    echo_qtro_frontend
)
