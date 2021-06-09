# Generated from imports.pro.

if(TARGET Qt::InterfaceFramework)
    add_subdirectory(core)
endif()
if(TARGET Qt::IfMedia)
    add_subdirectory(media)
endif()
if(TARGET Qt::IfVehicleFunctions) # special case
    add_subdirectory(vehiclefunctions)
endif()
