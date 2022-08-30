#####################################################################
## simple_plugin Generic Library:
#####################################################################

qt_add_plugin(simplePlugin
    PLUGIN_TYPE interfaceframework
)

target_sources(simplePlugin PRIVATE
    simpleplugin.cpp
    simpleplugin.h
)

qt_autogen_tools_initial_setup(simplePlugin)

set_target_properties(simplePlugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(simplePlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
