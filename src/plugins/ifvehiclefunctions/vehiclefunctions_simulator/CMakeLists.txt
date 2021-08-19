# Generated from vehiclefunctions_simulator.pro.

#####################################################################
## QtIfVehicleFunctionsSimulatorPlugin Plugin:
#####################################################################

qt_internal_add_plugin(vehicle_simulator_plugin
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

qt_internal_add_resource(vehicle_simulator_plugin "qml"
    PREFIX
        "/simulation"
    FILES
        ${qml_resource_files}
)


# Interface Framework Generator:
qt_ifcodegen_extend_target(vehicle_simulator_plugin
    IDL_FILES ../../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE backend_simulator
    MODULE_NAME QtIfVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions_simulator.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ifvehiclefunctions"
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
