# Generated from vehiclefunctions.pro.

#####################################################################
## vehiclefunctions Plugin:
#####################################################################

# Interface Framework Generator:
qt6_ifcodegen_import_variables(VEHICLEFUNCTIONS
    IDL_FILES ../../ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE qmlplugin
    MODULE_NAME QtIfVehicleFunctions
)

if (NOT VEHICLEFUNCTIONS_URI)
    return()
endif()

qt_internal_add_qml_module(qtifvehiclefunctionsplugin
    URI ${VEHICLEFUNCTIONS_URI}
    VERSION ${VEHICLEFUNCTIONS_VERSION}
    PLUGIN_TARGET qtifvehiclefunctionsplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    NO_GENERATE_QMLDIR
    SOURCES
        ${VEHICLEFUNCTIONS_SOURCES}
    LIBRARIES
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

#### Keys ignored in scope 1:.:.:vehiclefunctions.pro:<TRUE>:
# CXX_MODULE = "qml"
# QML_IMPORT_VERSION = "1.0"
# QT_FOR_CONFIG = "interfaceframework"
# TARGETPATH = "$$replace(URI, \\., /)"
# qml2build.files = "-$$_PRO_FILE_PWD_/qmldir" "-$$_PRO_FILE_PWD_/plugins.qmltypes"
# qmldir2build.files = "-$$_PRO_FILE_PWD_/qmldir" "-$$_PRO_FILE_PWD_/plugins.qmltypes"

## Scopes:
#####################################################################

#### Keys ignored in scope 3:.:.:vehiclefunctions.pro:NOT PWD___equals____ss_OUT_PWD:
# qmldir.base = <EMPTY>
# qmldir.files = "-$$_PRO_FILE_PWD_/qmldir"
# qmlfiles.base = <EMPTY>
# qmlfiles.files = "-$$_PRO_FILE_PWD_/qmldir"
