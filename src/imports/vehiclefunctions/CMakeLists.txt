# Generated from vehiclefunctions.pro.

#####################################################################
## vehiclefunctions Plugin:
#####################################################################

set(CMAKE_AUTOMOC ON)

# Interface Framework Generator:
qt_ifcodegen(qtifvehiclefunctionsplugin
    QFACE_SOURCES ../../ifvehiclefunctions/ifvehiclefunctions.qface
    QFACE_FORMAT qmlplugin
    QFACE_MODULE_NAME QtIfVehicleFunctions
)

# special case begin
target_link_libraries(qtifvehiclefunctionsplugin PUBLIC
    Qt::InterfaceFramework
    Qt::IfVehicleFunctions
)
# special case end

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
