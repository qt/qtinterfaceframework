set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()
qt_internal_add_test(tst_qface_include
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    LIBRARIES
        include_test_common
        include_test_frontend
    NO_BATCH
)
set_target_properties(tst_qface_include PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(tst_qface_include
    IDL_FILES ../../include-test.qface
    TEMPLATE test
    IDL_IMPORT_PATH ../../qface\ imports
)
