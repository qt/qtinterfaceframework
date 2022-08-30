set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt6_add_library(include_test_frontend)
set_target_properties(include_test_frontend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(include_test_frontend
    IDL_FILES ../../include-test.qface
    TEMPLATE frontend
    IMPORT_PATH ../../qface\ imports
)

target_link_libraries(include_test_frontend PUBLIC
    include_test_common
)
