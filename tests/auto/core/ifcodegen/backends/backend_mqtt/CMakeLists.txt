set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_ifcodegen_add_plugin(echo_backend_mqtt
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_mqtt
)
set_target_properties(echo_backend_mqtt PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(echo_backend_mqtt PUBLIC
    echo_qtro_frontend
)
