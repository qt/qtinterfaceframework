# Generated from basic.pro.

#####################################################################
## tst_vehiclefunctions_basic Test:
#####################################################################

file(TOUCH ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp)
qt_internal_add_test(tst_vehiclefunctions_basic
    SOURCES
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_dummy.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::InterfaceFramework
        Qt::IfVehicleFunctions
)

# Interface Framework Generator:
qt_ifcodegen(tst_vehiclefunctions_basic
    QFACE_SOURCES ../../../../src/ifvehiclefunctions/ifvehiclefunctions.qface
    QFACE_FORMAT test
)

#### Keys ignored in scope 1:.:.:basic.pro:<TRUE>:
# QMAKE_PROJECT_NAME = "$$TARGET"
