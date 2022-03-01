# Generated from frontend.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt6_add_library(echo_noprivate_frontend)
set_target_properties(echo_noprivate_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_noprivate_frontend
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE frontend
    ANNOTATION_FILES ../../no-private.yaml
)
