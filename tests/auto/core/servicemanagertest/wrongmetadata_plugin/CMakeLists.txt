# Generated from wrongmetadata_plugin.pro.

#####################################################################
## wrongmetadata_plugin Generic Library:
#####################################################################

qt_internal_add_plugin(wrongmetadataPlugin
    OUTPUT_NAME wrongmetadata_plugin
    PLUGIN_TYPE interfaceframework
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework/"
    SOURCES
        wrongmetadataplugin.cpp wrongmetadataplugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::InterfaceFramework
)

#### Keys ignored in scope 1:.:.:wrongmetadata_plugin.pro:<TRUE>:
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(wrongmetadataPlugin)
