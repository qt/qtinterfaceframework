set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_library(echo_frontend)
set_target_properties(echo_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(echo_frontend
    IDL_FILES ../../org.example.echo.qface
    PREFIX ECHO
    TEMPLATE frontend
)

qt_add_qml_module(echo_frontend
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../imports/${ECHO_URI_PATH}"
    URI ${ECHO_URI}
    VERSION ${ECHO_VERSION}
    RESOURCE_PREFIX "/"
)
