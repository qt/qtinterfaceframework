
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
