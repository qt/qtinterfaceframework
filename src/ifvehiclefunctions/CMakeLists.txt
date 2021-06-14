# Generated from ifvehiclefunctions.pro.

# special case begin
#####################################################################
## IfVehicleFunctions Module:
#####################################################################

qt_ifcodegen_generate(
    QFACE_SOURCES ifvehiclefunctions.qface
    QFACE_FORMAT frontend
    QFACE_MODULE_NAME QtIfVehicleFunctions
    QFACE_HEADERS_OUTPUT_DIR ${QT_BUILD_DIR}/src/ifvehiclefunctions
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
    QFACE_SOURCES ifvehiclefunctions.qface
)

# special case end

#### Keys ignored in scope 1:.:.:ifvehiclefunctions.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# OTHER_FILES = "$$PWD/doc/*.qdocconf" "$$PWD/doc/src/*.qdoc" "$$PWD/doc/snippets/*" "$$PWD/doc/images/*"
# QT_FOR_CONFIG = "interfaceframework"

## Scopes:
#####################################################################
