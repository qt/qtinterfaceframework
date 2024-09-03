set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_ifcodegen_add_plugin(echo_backend_simulator_static
    STATIC
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_simulator
    ANNOTATION_FILES annotation.yaml
)

target_link_libraries(echo_backend_simulator_static PUBLIC
    echo_qtro_frontend
)
