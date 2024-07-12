set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/interfaceframework/qface-tutorial/chapter5-ipc")

find_package(Qt6 REQUIRED COMPONENTS Core InterfaceFramework Quick)

qt_add_executable(chapter5-ipc-server)
set_target_properties(chapter5-ipc-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(chapter5-ipc-server
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE server_qtro_simulator
)

set_target_properties(chapter5-ipc-server PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

target_link_libraries(chapter5-ipc-server PUBLIC
    libIc_chapter5
)

# Resources:
set(simulation_resource_files
    "../backend_simulator/simulation.qml"
)

qt_add_resources(chapter5-ipc-server "simulation"
    PREFIX
        "/"
    BASE
        "../backend_simulator"
    FILES
        ${simulation_resource_files}
)

install(TARGETS chapter5-ipc-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
