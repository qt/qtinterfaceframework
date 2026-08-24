qt_ifcodegen_add_qml_module(simu
    IDL_FILES ../simu.qface
    TEMPLATE frontend
    OUTPUT_DIRECTORY ../imports/Simu
)

target_link_libraries(
    simu
    PRIVATE Qt::InterfaceFrameworkPrivate Qt::Quick
)

set_target_properties(
    simu
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..
               VERSION "1.0.0"
               SOVERSION "1"
)
