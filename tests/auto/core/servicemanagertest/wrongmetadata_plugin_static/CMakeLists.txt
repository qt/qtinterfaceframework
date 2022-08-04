# Generated from wrongmetadata_plugin_static.pro.

#####################################################################
## wrongmetadata_plugin_static Generic Library:
#####################################################################

qt_add_plugin(wrongmetadataStaticPlugin
    PLUGIN_TYPE interfaceframework
    CLASS_NAME WrongMetadataStaticPlugin
    STATIC
)

target_sources(wrongmetadataStaticPlugin PRIVATE
    wrongmetadatastaticplugin.cpp
    wrongmetadatastaticplugin.h
)

qt_autogen_tools_initial_setup(wrongmetadataStaticPlugin)

target_link_libraries(wrongmetadataStaticPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
