#####################################################################
## QtIfVehicleFunctionsSimulatorPlugin Plugin:
#####################################################################

qt_internal_add_plugin(VehicleSimulatorPlugin
    OUTPUT_NAME vehiclefunction_simulator
    PLUGIN_TYPE interfaceframework
    DEFAULT_IF FALSE
    LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Resources:
set(qml_resource_files
    "QIfConcreteWindowControlSimulation.qml"
    "simulation.qml"
)

qt_internal_add_resource(VehicleSimulatorPlugin "qml"
    PREFIX
        "/simulation"
    FILES
        ${qml_resource_files}
)


# Interface Framework Generator:
qt_ifcodegen_extend_target(VehicleSimulatorPlugin
    IDL_FILES ../../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE backend_simulator
    MODULE_NAME QtIfVehicleFunctions
)
