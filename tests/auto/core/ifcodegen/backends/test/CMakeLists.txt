set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

function(internal_add_backend_test target sources)
    qt_internal_add_test(${target}
        SOURCES
            ${sources} backendstestbase.cpp backendstestbase.h
        LIBRARIES
            echo_qtro_frontend
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/../
        TESTDATA simulation.qml minimal_simulation_data.json server.conf
        NO_BATCH
    )
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
endfunction()

internal_add_backend_test(tst_simulation_backend_static
    tst_simulation_backend_static.cpp
)
target_link_libraries(tst_simulation_backend_static PRIVATE echo_backend_simulator_static)

internal_add_backend_test(tst_simulation_backend
    tst_simulation_backend.cpp
)

if (QT_FEATURE_remoteobjects)
    internal_add_backend_test(tst_qtro_backend
        tst_qtro_backend.cpp
    )
    target_link_libraries(tst_qtro_backend PRIVATE echo_backend_qtro_static)
endif()

add_custom_target(tst_backends_extra_files
    SOURCES
        simulation.qml
        minimal_simulation_data.json
)
