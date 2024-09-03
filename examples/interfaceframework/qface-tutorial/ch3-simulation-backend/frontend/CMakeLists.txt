
qt_ifcodegen_add_qml_module(libIc_ch3
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

set_target_properties(libIc_ch3 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
set_target_properties(libIc_ch3 PROPERTIES OUTPUT_NAME "InstrumentCluster")

install(TARGETS libIc_ch3
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

if (TARGET libIc_ch3plugin)
    set_target_properties(libIc_ch3plugin
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../../../"
    )

    install(
        TARGETS libIc_ch3plugin
        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
    )
endif()

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/Example/If/InstrumentClusterModule/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/Example/If/InstrumentClusterModule"
)
