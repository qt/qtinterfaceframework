
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
