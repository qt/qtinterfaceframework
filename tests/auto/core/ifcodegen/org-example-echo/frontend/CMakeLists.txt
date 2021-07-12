# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(echo_frontend)
set_target_properties(echo_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_frontend
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE frontend
)

target_link_libraries(echo_frontend PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)
