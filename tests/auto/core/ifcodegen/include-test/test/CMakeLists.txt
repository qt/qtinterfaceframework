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
qt6_ifcodegen_extend_target(tst_qface_include
    IDL_FILES ../../include-test.qface
    TEMPLATE test
    IMPORT_PATH ../../qface\ imports
)

if (ANDROID)
    set(libs
        include_test_common
        include_test_frontend
    )
    set(extra_libs)
    list(APPEND extra_libs
         "${CMAKE_CURRENT_BINARY_DIR}/../frontend/libinclude_test_frontend_${CMAKE_ANDROID_ARCH_ABI}.so")
    list(APPEND extra_libs
         "${CMAKE_CURRENT_BINARY_DIR}/../common/libinclude_test_common_${CMAKE_ANDROID_ARCH_ABI}.so")
    set_target_properties(tst_qface_include PROPERTIES
        QT_ANDROID_EXTRA_LIBS "${extra_libs}"
    )

    qt6_android_generate_deployment_settings(tst_qface_include)
endif ()
