set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noanno_qtro)

# Interface Framework Generator:
qt_ifcodegen_extend_target(echo_noanno_qtro
    IDL_FILES ../../org.example.echo.noannotation.qface
    TEMPLATE backend_qtro
)

target_link_libraries(echo_noanno_qtro PUBLIC
    echo_noanno_frontend
)
