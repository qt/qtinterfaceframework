#####################################################################
## QtIfVehicleFunctionsQtROPlugin Plugin:
#####################################################################

qt_internal_add_plugin(VehicleQtROPlugin
    OUTPUT_NAME vehiclefunction_qtro
    PLUGIN_TYPE interfaceframework
    DEFAULT_IF FALSE
    LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Interface Framework Generator:
qt_ifcodegen_extend_target(VehicleQtROPlugin
    IDL_FILES ../../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE backend_qtro
    MODULE_NAME QtIfVehicleFunctions
)
