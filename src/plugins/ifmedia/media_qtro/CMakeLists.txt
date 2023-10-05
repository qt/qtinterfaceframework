#####################################################################
## MediaQtROPlugin Plugin:
#####################################################################

qt_internal_add_plugin(MediaQtROPlugin
    OUTPUT_NAME media_qtro
    PLUGIN_TYPE interfaceframework
    DEFAULT_IF FALSE
    SOURCES
        mediadiscoverybackend.cpp mediadiscoverybackend.h
        mediaindexerbackend.cpp mediaindexerbackend.h
        mediaplayerbackend.cpp mediaplayerbackend.h
        mediaplugin.cpp mediaplugin.h
        filterandbrowsemodel.cpp filterandbrowsemodel.h
        usbdevice.cpp usbdevice.h
    LIBRARIES
        Qt::InterfaceFramework
        Qt::IfMedia
        Qt::RemoteObjects
        Qt::IfRemoteObjectsHelper
        Qt::IfRemoteObjectsHelperPrivate
)
qt6_add_repc_replicas(MediaQtROPlugin
    ../../../tools/media-simulation-server/qifmediaindexer.rep
    ../../../tools/media-simulation-server/qifmediaplayer.rep
    ../../../tools/media-simulation-server/qiffilterandbrowsemodel.rep
    ../../../tools/media-simulation-server/qifmediadiscoverymodel.rep
)
