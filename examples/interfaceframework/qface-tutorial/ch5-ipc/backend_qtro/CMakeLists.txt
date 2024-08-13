
qt_ifcodegen_add_plugin(ic_chapter5_qtro
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE backend_qtro
)

set_target_properties(ic_chapter5_qtro PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(ic_chapter5_qtro PUBLIC
    libIc_chapter5
)

install(TARGETS ic_chapter5_qtro
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/interfaceframework"
)
