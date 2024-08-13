
qt_ifcodegen_add_plugin(ic_ch4_simulation
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE backend_simulator
)

set_target_properties(ic_ch4_simulation PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(ic_ch4_simulation PUBLIC
    libIc_ch4
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/backend_simulator/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

qt_add_resources(ic_ch4_simulation "simulation"
    PREFIX
        "/"
    FILES
       "simulation.qml"
)

install(TARGETS ic_ch4_simulation
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
