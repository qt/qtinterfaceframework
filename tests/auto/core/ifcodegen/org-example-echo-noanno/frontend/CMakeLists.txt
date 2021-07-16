# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt6_add_library(echo_noanno_frontend)
set_target_properties(echo_noanno_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_noanno_frontend
    IDL_FILES ../../org.example.echo.noannotation.qface
    TEMPLATE frontend
)

target_link_libraries(echo_noanno_frontend PUBLIC
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
    Qt::InterfaceFrameworkPrivate
    Qt::Qml
    Qt::Quick
)
