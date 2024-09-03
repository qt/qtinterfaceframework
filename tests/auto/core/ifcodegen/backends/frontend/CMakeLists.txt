set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_library(echo_qtro_frontend)
set_target_properties(echo_qtro_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(echo_qtro_frontend
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE frontend
)
