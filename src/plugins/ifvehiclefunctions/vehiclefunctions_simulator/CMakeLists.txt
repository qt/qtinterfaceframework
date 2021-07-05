# Generated from vehiclefunctions_simulator.pro.

#####################################################################
## QtIfVehicleFunctionsSimulatorPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QtIfVehicleFunctionsSimulatorPlugin
    OUTPUT_NAME vehiclefunction_simulator
    TYPE interfaceframework
    DEFAULT_IF FALSE
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Resources:
set(qml_resource_files
    "QIfConcreteWindowControlSimulation.qml"
    "simulation.qml"
)

qt_internal_add_resource(QtIfVehicleFunctionsSimulatorPlugin "qml"
    PREFIX
        "/simulation"
    FILES
        ${qml_resource_files}
)


# Interface Framework Generator:
qt_ifcodegen(QtIfVehicleFunctionsSimulatorPlugin
    IFCODEGEN_SOURCES ../../../ifvehiclefunctions/ifvehiclefunctions.qface
    IFCODEGEN_TEMPLATE backend_simulator
    IFCODEGEN_MODULE_NAME QtIfVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions_simulator.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ifvehiclefunctions"
# QML_IMPORT_PATH = "$$OUT_PWD/qml"
