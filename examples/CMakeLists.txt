qt_examples_build_begin(EXTERNAL_BUILD)

if(TARGET Qt::Gui AND TARGET Qt::InterfaceFramework)
    add_subdirectory(interfaceframework)
endif()
if(TARGET Qt::Gui AND TARGET Qt::IfVehicleFunctions)
    add_subdirectory(ifvehiclefunctions)
endif()
if(TARGET Qt::Gui AND TARGET Qt::IfMedia)
    add_subdirectory(ifmedia)
endif()

qt_examples_build_end()
