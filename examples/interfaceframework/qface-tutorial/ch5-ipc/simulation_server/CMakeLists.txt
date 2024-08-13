set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_executable(chapter5-ipc-server WIN32)
set_target_properties(chapter5-ipc-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(chapter5-ipc-server
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE server_qtro_simulator
)

target_link_libraries(chapter5-ipc-server PUBLIC
    libIc_chapter5
)

qt_add_resources(chapter5-ipc-server "simulation"
    PREFIX
        "/"
    BASE
        "../backend_simulator"
    FILES
        "../backend_simulator/simulation.qml"
)

install(TARGETS chapter5-ipc-server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
