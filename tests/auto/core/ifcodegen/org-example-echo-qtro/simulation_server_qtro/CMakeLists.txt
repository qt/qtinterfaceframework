set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(org-example-echo-qtro-simulation-server)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(org-example-echo-qtro-simulation-server
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE server_qtro_simulator
)

target_link_libraries(org-example-echo-qtro-simulation-server PUBLIC
    echo_qtro_frontend
)
