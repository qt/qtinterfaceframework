qt_add_library(simu)

qt_ifcodegen_extend_target(
    simu
    IDL_FILES ../simu.qface
    TEMPLATE frontend
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
