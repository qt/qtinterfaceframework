# Generated from vehiclefunctions_qtro.pro.

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

# Workaround for QTBUG-105215
# Plugins usually don't take PUBLIC_LIBRARIES, but in this case we have to force the creation
# of a Qt6VehicleQtROPluginDependencies.cmake file to find the Qt6IfRemoteObjectsHelperPrivate
# package because it gets (wrongly) added as a public dependency by qt_ifcodegen_extend_target,
# which causes the library to appear in the plugin's exported INTERFACE_LINK_LIBRARIES.
# Qt only writes find_depednency calls for Qt packages that are specified via
# qt_internal_extend_target, not via target_link_libraries.
# So the dependency specified via qt_ifcodegen_extend_target is not seen.
qt_internal_extend_target(VehicleQtROPlugin
    PUBLIC_LIBRARIES
        Qt::IfRemoteObjectsHelperPrivate
)

# Interface Framework Generator:
qt_ifcodegen_extend_target(VehicleQtROPlugin
    IDL_FILES ../../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE backend_qtro
    MODULE_NAME QtIfVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions_qtro.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# PLUGIN_EXTENDS = "ifvehiclefunctions"
