set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_ifcodegen_add_plugin(echo_backend_qtro_static
    STATIC
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE backend_qtro
    ANNOTATION_FILES annotation.yaml
)

target_link_libraries(echo_backend_qtro_static PUBLIC
    echo_qtro_frontend
)
