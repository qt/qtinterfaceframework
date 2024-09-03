set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_plugin(include_test_simulator)
set_target_properties(include_test_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

# Interface Framework Generator:
qt_ifcodegen_extend_target(include_test_simulator
    IDL_FILES ../../include-test.qface
    TEMPLATE backend_simulator
    IDL_IMPORT_PATH ../../qface\ imports
)

target_link_libraries(include_test_simulator PUBLIC
    include_test_frontend
)
