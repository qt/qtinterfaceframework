set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt6_add_library(echo_flat_frontend)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_flat_frontend
    IDL_FILES ../org.example.echo.qface
    PREFIX ECHO
    TEMPLATE frontend
)

####### BACKEND #######

qt_add_plugin(echo_flat_simulator)
set_target_properties(echo_flat_simulator PROPERTIES LIBRARY_OUTPUT_DIRECTORY interfaceframework)
target_sources(echo_flat_simulator PRIVATE
    backend_simulator.cpp
)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(echo_flat_simulator
    IDL_FILES ../org.example.echo.qface
    TEMPLATE backend_simulator
)

target_link_libraries(echo_flat_simulator PUBLIC
    echo_flat_frontend
)

####### TEST #######

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_internal_add_test(tst_flat-cmake-hierarchy
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    LIBRARIES
        echo_flat_frontend
)

# Interface Framework Generator:
qt6_ifcodegen_extend_target(tst_flat-cmake-hierarchy
    IDL_FILES ../org.example.echo.qface
    TEMPLATE test
)
