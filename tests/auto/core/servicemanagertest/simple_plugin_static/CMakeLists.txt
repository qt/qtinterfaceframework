# Generated from simple_plugin_static.pro.

#####################################################################
## simple_plugin_static Generic Library:
#####################################################################

qt_add_plugin(simpleStaticPlugin
    PLUGIN_TYPE interfaceframework
    CLASS_NAME SimpleStaticPlugin
    STATIC
)

target_sources(simpleStaticPlugin PRIVATE
    simplestaticplugin.cpp
    simplestaticplugin.h
)

qt_autogen_tools_initial_setup(simpleStaticPlugin)

target_link_libraries(simpleStaticPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
