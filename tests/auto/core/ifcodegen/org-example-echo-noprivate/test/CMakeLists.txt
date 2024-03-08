set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp")
    file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
endif()
qt_internal_add_test(tst_org-example-echo-noprivate
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    LIBRARIES
        echo_noprivate_frontend
    NO_BATCH
)
set_target_properties(tst_org-example-echo-noprivate PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(tst_org-example-echo-noprivate
    IDL_FILES ../../org.example.echo.qface
    TEMPLATE test
    ANNOTATION_FILES ../../no-private.yaml
)
