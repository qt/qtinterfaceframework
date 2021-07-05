# Generated from test.pro.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_internal_add_test(tst_org-example-echo
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    PUBLIC_LIBRARIES
        echo_frontend
        Qt::Core
        Qt::InterfaceFramework
        Qt::Test
)
set_target_properties(tst_org-example-echo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen(tst_org-example-echo
    IFCODEGEN_SOURCES ../../org.example.echo.qface
    IFCODEGEN_TEMPLATE test
)
