# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen(echo_frontend_qmlplugin # special case
    IFCODEGEN_SOURCES ../../org.example.echo.qface
    IFCODEGEN_TEMPLATE qmlplugin
)

target_link_libraries(echo_frontend_qmlplugin PRIVATE # special case
    echo_frontend
    Qt::InterfaceFramework
    Qt::IfVehicleFunctions
)
