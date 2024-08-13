
qt_add_library(libIc_ch1)
set_target_properties(libIc_ch1 PROPERTIES OUTPUT_NAME "InstrumentCluster")
set_target_properties(libIc_ch1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

# Interface Framework Generator:
qt_ifcodegen_extend_target(libIc_ch1
    IDL_FILES ../instrument-cluster.qface
    TEMPLATE frontend
)

set(import_path "${CMAKE_CURRENT_BINARY_DIR}/frontend/qml")
if (NOT ${import_path} IN_LIST QML_IMPORT_PATH)
    list (APPEND QML_IMPORT_PATH "${import_path}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE INTERNAL "" FORCE)
endif()

install(TARGETS libIc_ch1
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
