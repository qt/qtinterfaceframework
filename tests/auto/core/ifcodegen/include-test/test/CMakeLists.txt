# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_internal_add_test(tst_qface_include
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    PUBLIC_LIBRARIES
        include_test_common
        include_test_frontend
        Qt::Core
        Qt::InterfaceFramework
        Qt::Test
)
set_target_properties(tst_qface_include PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(tst_qface_include
    IFCODEGEN_SOURCES ../../include-test.qface
    IFCODEGEN_TEMPLATE test
    IFCODEGEN_IMPORT_PATH ../../qface\ imports
)
