# Generated from auto.pro.

if(NOT ANDROID) # QTBUG-104973
    if(TARGET Qt::InterfaceFramework)
        add_subdirectory(core)
    endif()
    if(TARGET Qt::IfVehicleFunctions)
        add_subdirectory(vehiclefunctions)
    endif()
endif()
