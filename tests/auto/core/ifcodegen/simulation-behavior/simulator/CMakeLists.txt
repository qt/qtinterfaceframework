qt_add_plugin(simu_simulator
    OUTPUT_NAME simu_simulator
    PLUGIN_TYPE interfaceframework
)

qt_ifcodegen_extend_target(
    simu_simulator
    IDL_FILES ../simu.qface
    TEMPLATE backend_simulator
)

target_link_libraries(
    simu_simulator
    PRIVATE Qt::InterfaceFramework simu
)

set_target_properties(
    simu_simulator
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework
)

qt_add_resources(
    simu_simulator
    "simulation"
    PREFIX "/"
    FILES simulation.qml
)
