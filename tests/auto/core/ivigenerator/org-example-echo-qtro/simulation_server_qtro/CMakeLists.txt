# Generated from simulation_server_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(org-example-echo-qtro-simulation-server)

# Interface Framework Generator:
qt6_ifcodegen(org-example-echo-qtro-simulation-server
    QFACE_SOURCES ../../org.example.echo.qface
    QFACE_FORMAT server_qtro_simulator
)

target_link_libraries(org-example-echo-qtro-simulation-server PUBLIC
    echo_qtro_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::Qml
    Qt::Quick
)
