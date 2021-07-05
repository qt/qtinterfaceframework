# Generated from qmlplugin.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Interface Framework Generator:
qt6_ifcodegen(echo_frontend_noanno_qmlplugin # special case
    IFCODEGEN_SOURCES ../../org.example.echo.noannotation.qface
    IFCODEGEN_TEMPLATE qmlplugin
)

target_link_libraries(echo_frontend_noanno_qmlplugin PRIVATE # special case
    echo_noanno_frontend
    Qt::InterfaceFramework
    Qt::IfVehicleFunctions
)
