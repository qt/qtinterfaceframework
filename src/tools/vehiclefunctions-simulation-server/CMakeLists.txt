# Generated from vehiclefunctions-simulation-server.pro.

#####################################################################
## ifvehiclefunctions-simulation-server Tool:
#####################################################################

qt_get_tool_target_name(target_name ifvehiclefunctions-simulation-server)
qt_internal_add_tool(${target_name}
     TOOLS_TARGET IfVehicleFunctions
     LIBRARIES
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
     PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Resources:
set(qml_resource_files
    "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator/QIfConcreteWindowControlSimulation.qml"
    "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator/simulation.qml"
)

qt_internal_add_resource(${target_name} "qml"
    PREFIX
        "/simulation"
    BASE
        "../../plugins/ifvehiclefunctions/vehiclefunctions_simulator"
    FILES
        ${qml_resource_files}
)


# Interface Framework Generator:
# When cross-compiling the tool might not be compiled and the target
# doesn't exist in this case
if (TARGET ${target_name})
    qt6_ifcodegen_extend_target(${target_name}
        IDL_FILES ../../ifvehiclefunctions/ifvehiclefunctions.qface
        TEMPLATE server_qtro_simulator
        MODULE_NAME QtIfVehicleFunctions
    )
endif()

#### Keys ignored in scope 1:.:.:vehiclefunctions-simulation-server.pro:<TRUE>:
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
# QT_FOR_CONFIG = "interfaceframework"
# TEMPLATE = "app"

## Scopes:
#####################################################################
