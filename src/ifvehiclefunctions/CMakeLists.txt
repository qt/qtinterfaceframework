# Generated from ifvehiclefunctions.pro.

# special case begin
#####################################################################
## IfVehicleFunctions Module:
#####################################################################

qt_ifcodegen_generate(
    IFCODEGEN_SOURCES ifvehiclefunctions.qface
    IFCODEGEN_TEMPLATE frontend
    IFCODEGEN_MODULE_NAME QtIfVehicleFunctions
    IFCODEGEN_HEADERS_OUTPUT_DIR ${QT_BUILD_DIR}/src/ifvehiclefunctions
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

qt_ifcodegen_include(IfVehicleFunctions
    IFCODEGEN_SOURCES ifvehiclefunctions.qface
)

#####################################################################
## Documentation
#####################################################################

file(GLOB_RECURSE allDocFiles "doc/*.qdoc" "doc/*.png" "doc/*.qdocconf")
add_custom_target(IfVehicleFunctions_Documentation SOURCES
    ${allDocFiles}
)
