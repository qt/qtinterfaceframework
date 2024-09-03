set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_library(include_test_frontend)
set_target_properties(include_test_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(include_test_frontend
    IDL_FILES ../../include-test.qface
    TEMPLATE frontend
    IDL_IMPORT_PATH ../../qface\ imports
)

target_link_libraries(include_test_frontend PUBLIC
    include_test_common
)
