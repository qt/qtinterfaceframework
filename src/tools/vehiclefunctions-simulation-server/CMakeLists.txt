#####################################################################
## ifvehiclefunctions-simulation-server app:
#####################################################################

qt_internal_add_app(ifvehiclefunctions-simulation-server
     LIBRARIES
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Resources:
set(qml_resource_files
    "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator/QIfConcreteWindowControlSimulation.qml"
    "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator/simulation.qml"
)

qt_internal_add_resource(ifvehiclefunctions-simulation-server "qml"
    PREFIX
        "/simulation"
    BASE
        "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator"
    FILES
        ${qml_resource_files}
)


qt6_ifcodegen_extend_target(ifvehiclefunctions-simulation-server
    IDL_FILES ../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE server_qtro_simulator
    MODULE_NAME QtIfVehicleFunctions
)
