# Generated from ifvehiclefunctions.pro.

# special case begin
#####################################################################
## IfVehicleFunctions Module:
#####################################################################

qt_ifcodegen_generate(
    IDL_FILES ifvehiclefunctions.qface
    TEMPLATE frontend
    MODULE_NAME QtIfVehicleFunctions
    EXTRA_HEADERS_OUTPUT_DIR ${QT_BUILD_DIR}/src/ifvehiclefunctions
)
make_directory(${QT_BUILD_DIR}/src/ifvehiclefunctions)

qt_internal_add_module(IfVehicleFunctions
    LIBRARIES
        Qt::CorePrivate
        Qt::InterfaceFrameworkPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::InterfaceFrameworkPrivate
)

qt_ifcodegen_extend_target(IfVehicleFunctions
    NO_GENERATE
    IDL_FILES ifvehiclefunctions.qface
)

qt_ifcodegen_import_variables(VEHICLEFUNCTIONS
    NO_GENERATE
    IDL_FILES ifvehiclefunctions.qface
)

qt_internal_add_qml_module(IfVehicleFunctions
    URI ${VEHICLEFUNCTIONS_URI}
    VERSION ${VEHICLEFUNCTIONS_VERSION}
    CLASS_NAME QtIfVehicleFunctionsPlugin
    PLUGIN_TARGET qtifvehiclefunctionsplugin
    IMPORTS QtInterfaceFramework/auto
)

#####################################################################
## Documentation
#####################################################################

file(GLOB_RECURSE allDocFiles "doc/*.qdoc" "doc/*.png" "doc/*.qdocconf")
add_custom_target(IfVehicleFunctions_Documentation SOURCES
    ${allDocFiles}
)
