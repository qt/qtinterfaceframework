set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_ifcodegen_add_plugin(echo_qtro_simulator
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_simulator
)
set_target_properties(echo_qtro_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(echo_qtro_simulator PUBLIC
    echo_qtro_frontend
)
