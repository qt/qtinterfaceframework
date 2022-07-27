# Generated from simple_plugin.pro.

#####################################################################
## simple_plugin Generic Library:
#####################################################################

qt_internal_add_cmake_library(simple_plugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../interfaceframework/"
    SOURCES
        simpleplugin.cpp simpleplugin.h
    LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::InterfaceFramework
)

#### Keys ignored in scope 1:.:.:simple_plugin.pro:<TRUE>:
# DISTFILES = "simple_plugin.json"
# TEMPLATE = "lib"

## Scopes:
#####################################################################

qt_autogen_tools_initial_setup(simple_plugin)
