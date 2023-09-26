# Generated from wrong_plugin.pro.

#####################################################################
## wrong_plugin Generic Library:
#####################################################################

qt_add_plugin(wrongPlugin
    PLUGIN_TYPE interfaceframework
)

target_sources(wrongPlugin PRIVATE
    wrongplugin.cpp
    wrongplugin.h
)

qt_autogen_tools_initial_setup(wrongPlugin)

set_target_properties(wrongPlugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(wrongPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
