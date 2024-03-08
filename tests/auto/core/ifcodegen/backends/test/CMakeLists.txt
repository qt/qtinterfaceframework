set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_internal_add_test(tst_backends
    SOURCES
        tst_backends.cpp tst_backends.h
    LIBRARIES
        echo_qtro_frontend
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/../
    TESTDATA simulation.qml minimal_simulation_data.json server.conf
    NO_BATCH
)

add_custom_target(tst_backends_extra_files
    SOURCES
        simulation.qml
        minimal_simulation_data.json
)

set_target_properties(tst_backends PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

