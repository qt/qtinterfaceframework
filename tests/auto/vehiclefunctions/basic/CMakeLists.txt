# Generated from basic.pro.

#####################################################################
## tst_vehiclefunctions_basic Test:
#####################################################################

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_internal_add_test(tst_vehiclefunctions_basic
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Interface Framework Generator:
qt_ifcodegen_extend_target(tst_vehiclefunctions_basic
    IDL_FILES ../../../../src/ifvehiclefunctions/ifvehiclefunctions.qface
    TEMPLATE test
)

#### Keys ignored in scope 1:.:.:basic.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
