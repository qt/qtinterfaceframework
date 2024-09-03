
qt_ifcodegen_add_qml_module(libIc_chapter6
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)
set_target_properties(libIc_chapter6 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_chapter6 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

install(TARGETS libIc_chapter6
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

if (TARGET libIc_chapter6plugin)
    set_target_properties(libIc_chapter6plugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS libIc_chapter6plugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/InstrumentClusterModule/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
)
