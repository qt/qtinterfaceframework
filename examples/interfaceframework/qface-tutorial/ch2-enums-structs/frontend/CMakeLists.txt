
qt_add_library(libIc_ch2)
set_target_properties(libIc_ch2 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_ch2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(libIc_ch2
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

install(TARGETS libIc_ch2
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
