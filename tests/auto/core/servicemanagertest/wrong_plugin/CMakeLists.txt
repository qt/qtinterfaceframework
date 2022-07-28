# Generated from wrong_plugin.pro.

#####################################################################
## wrong_plugin Generic Library:
#####################################################################

qt_internal_add_plugin(wrongPlugin
    OUTPUT_NAME wrong_plugin
    PLUGIN_TYPE interfaceframework
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework/"
    SOURCES
        wrongplugin.cpp wrongplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::InterfaceFramework
)

#### Keys ignored in scope 1:.:.:wrong_plugin.pro:<TRUE>:
# DISTFILES = "wrong_plugin.json"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(wrongPlugin)
