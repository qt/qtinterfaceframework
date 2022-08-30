#####################################################################
## IfVehicleFunctions Module:
#####################################################################

qt_ifcodegen_generate(
    IDL_FILES ifvehiclefunctions.qface
    TEMPLATE frontend
    MODULE_NAME QtIfVehicleFunctions
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
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
    PREFIX VEHICLEFUNCTIONS
    IDL_FILES ifvehiclefunctions.qface
    TEMPLATE frontend
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
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
