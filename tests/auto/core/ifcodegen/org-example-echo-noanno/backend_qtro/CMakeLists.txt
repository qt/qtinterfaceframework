# Generated from backend_qtro.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(echo_noanno_qtro)

# Interface Framework Generator:
qt6_ifcodegen(echo_noanno_qtro
    IFCODEGEN_SOURCES ../../org.example.echo.noannotation.qface
    IFCODEGEN_TEMPLATE backend_qtro
)

target_link_libraries(echo_noanno_qtro PUBLIC
    echo_noanno_frontend
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
