# Generated from simulation_server_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(org-example-echo-noanno-simu-server)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(org-example-echo-noanno-simu-server
    IDL_FILES ../../org.example.echo.noannotation.qface
    TEMPLATE server_qtro_simulator
)

target_link_libraries(org-example-echo-noanno-simu-server PUBLIC
    echo_noanno_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::Qml
    Qt::Quick
)
