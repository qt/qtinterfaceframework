# Generated from simple_plugin_static.pro.

#####################################################################
## simple_plugin_static Generic Library:
#####################################################################

qt_internal_add_plugin(simpleStaticPlugin
    OUTPUT_NAME simple_plugin_static
    PLUGIN_TYPE interfaceframework
    STATIC
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework/"
    SOURCES
        simplestaticplugin.cpp simplestaticplugin.h
    LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::InterfaceFramework
)

#### Keys ignored in scope 1:.:.:simple_plugin_static.pro:<TRUE>:
# DISTFILES = "simple_plugin.json"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(simpleStaticPlugin)

target_compile_definitions(simpleStaticPlugin PRIVATE QT_STATICPLUGIN)
