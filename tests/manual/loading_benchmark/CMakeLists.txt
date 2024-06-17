cmake_minimum_required(VERSION 3.16)

project(loading_benchmark VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Quick InterfaceFramework)
qt_policy(SET QTP0001 NEW)

qt_standard_project_setup()

set(INTERFACES 40)
set(PROPERTIES 100)

set(QFACE_CONTENT
"module Benchmark 1.0;

struct TestStruct {
    real value
    real value2
    real value3
    string string1
    string string2
    string string3
    bool condition
    bool condition2
    bool condition3
}

"
)
foreach(i RANGE ${INTERFACES})
    string(APPEND QFACE_CONTENT "interface Interface${i} {\n")
    foreach(j RANGE ${PROPERTIES})
        string(APPEND QFACE_CONTENT "   TestStruct prop${j}\n")
    endforeach()
    string(APPEND QFACE_CONTENT "}\n")
endforeach()

file(CONFIGURE OUTPUT benchmark.qface CONTENT "${QFACE_CONTENT}")

qt6_add_library(loading_benchmark_frontend)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(loading_benchmark_frontend
    IDL_FILES ${CMAKE_CURRENT_BINARY_DIR}/benchmark.qface
    TEMPLATE frontend
)

####### BACKEND #######

qt_add_plugin(loading_benchmark_simulator)
set_target_properties(loading_benchmark_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(loading_benchmark_simulator
    IDL_FILES ${CMAKE_CURRENT_BINARY_DIR}/benchmark.qface
    TEMPLATE backend_simulator
)

target_link_libraries(loading_benchmark_simulator PUBLIC
    loading_benchmark_frontend
)

qt_add_plugin(loading_benchmark_qtro)
set_target_properties(loading_benchmark_qtro PROPERTIES LIBRARY_OUTPUT_DIRECTORY interfaceframework)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(loading_benchmark_qtro
    IDL_FILES ${CMAKE_CURRENT_BINARY_DIR}/benchmark.qface
    TEMPLATE backend_qtro
)

target_link_libraries(loading_benchmark_qtro PUBLIC
    loading_benchmark_frontend
)

##### BENCHMARK #####

qt_add_executable(loading_benchmark
    main.cpp
)

qt_add_qml_module(loading_benchmark
    URI benchmark
    VERSION 1.0
    QML_FILES main.qml
)

target_link_libraries(loading_benchmark
    PRIVATE
        loading_benchmark_frontend
        Qt6::Quick
)

install(TARGETS loading_benchmark
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

install(TARGETS loading_benchmark_frontend
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

install(TARGETS loading_benchmark_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "/usr/bin/interfaceframework"
)
