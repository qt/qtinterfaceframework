set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noanno_mqtt)

# Interface Framework Generator:
qt_ifcodegen_extend_target(echo_noanno_mqtt
    IDL_FILES ../../org.example.echo.noannotation.qface
    TEMPLATE backend_mqtt
)

target_link_libraries(echo_noanno_mqtt PUBLIC
    echo_noanno_frontend
)
