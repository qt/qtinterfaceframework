# Generated from wrongmetadata_plugin_static.pro.

#####################################################################
## wrongmetadata_plugin_static Generic Library:
#####################################################################

qt_internal_add_plugin(wrongmetadataStaticPlugin
    OUTPUT_NAME wrongmetadata_plugin_static
    PLUGIN_TYPE interfaceframework
    STATIC
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework/"
    SOURCES
        wrongmetadatastaticplugin.cpp wrongmetadatastaticplugin.h
    LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::InterfaceFramework
)

#### Keys ignored in scope 1:.:.:wrongmetadata_plugin_static.pro:<TRUE>:
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(wrongmetadataStaticPlugin)

target_compile_definitions(wrongmetadataStaticPlugin PRIVATE QT_STATICPLUGIN)
