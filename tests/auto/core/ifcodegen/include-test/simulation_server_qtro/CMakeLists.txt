# Generated from simulation_server_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_executable(include-test-simulation-server)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(include-test-simulation-server
    IDL_FILES ../../include-test.qface
    TEMPLATE server_qtro_simulator
    IMPORT_PATH ../../qface\ imports
)

target_link_libraries(include-test-simulation-server PUBLIC
    include_test_common
    include_test_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::Qml
    Qt::Quick
)
