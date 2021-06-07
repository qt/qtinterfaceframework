# Generated from examples.pro.

qt_examples_build_begin()

if(TARGET Qt::Gui AND TARGET Qt::IviCore)
    add_subdirectory(ivicore)
endif()
if(TARGET Qt::Gui AND TARGET Qt::IviVehicleFunctions)
    add_subdirectory(ivivehiclefunctions)
endif()
if(TARGET Qt::Gui AND TARGET Qt::IviMedia)
    add_subdirectory(ivimedia)
endif()

qt_examples_build_end()
