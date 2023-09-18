# Generated from wrongmetadata_plugin.pro.

#####################################################################
## wrongmetadata_plugin Generic Library:
#####################################################################

qt_add_plugin(wrongmetadataPlugin
    PLUGIN_TYPE interfaceframework
)

target_sources(wrongmetadataPlugin PRIVATE
    wrongmetadataplugin.cpp
    wrongmetadataplugin.h
)

qt_autogen_tools_initial_setup(wrongmetadataPlugin)

set_target_properties(wrongmetadataPlugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../interfaceframework)

target_link_libraries(wrongmetadataPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::InterfaceFramework
)
