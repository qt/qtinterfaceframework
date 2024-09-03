
qt_ifcodegen_add_qml_module(libIc_ch4
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

set_target_properties(libIc_ch4 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_ch4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

install(TARGETS libIc_ch4
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

if (TARGET libIc_ch4plugin)
    set_target_properties(libIc_ch4plugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS libIc_ch4plugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/InstrumentClusterModule/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
)
