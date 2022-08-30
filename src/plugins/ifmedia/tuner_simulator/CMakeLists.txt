#####################################################################
## TunerPlugin Plugin:
#####################################################################

qt_internal_add_plugin(TunerPlugin
    OUTPUT_NAME tuner_simulator
    PLUGIN_TYPE interfaceframework
    DEFAULT_IF FALSE
    SOURCES
        amfmtunerbackend.cpp amfmtunerbackend.h
        searchandbrowsebackend.cpp searchandbrowsebackend.h
        tunerplugin.cpp tunerplugin.h
    LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
        Qt::IfMedia
)
