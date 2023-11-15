qt_examples_build_begin(EXTERNAL_BUILD)

if(TARGET Qt::Gui AND TARGET Qt::InterfaceFramework)
    add_subdirectory(interfaceframework)
endif()

qt_examples_build_end()
