# Generated from media_qtro.pro.

#####################################################################
## MediaQtROPlugin Plugin:
#####################################################################

qt_internal_add_plugin(MediaQtROPlugin
    OUTPUT_NAME media_qtro
    TYPE interfaceframework
    DEFAULT_IF FALSE
    SOURCES
        mediadiscoverybackend.cpp mediadiscoverybackend.h
        mediaindexerbackend.cpp mediaindexerbackend.h
        mediaplayerbackend.cpp mediaplayerbackend.h
        mediaplugin.cpp mediaplugin.h
        filterandbrowsemodel.cpp filterandbrowsemodel.h
        usbdevice.cpp usbdevice.h
    PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::IfMedia
        Qt::RemoteObjects
        Qt::IfRemoteObjectsHelperPrivate
)
qt6_add_repc_replicas(MediaQtROPlugin
    ../../../tools/media-simulation-server/qifmediaindexer.rep
    ../../../tools/media-simulation-server/qifmediaplayer.rep
    ../../../tools/media-simulation-server/qiffilterandbrowsemodel.rep
    ../../../tools/media-simulation-server/qifmediadiscoverymodel.rep
)

#### Keys ignored in scope 1:.:.:media_qtro.pro:<TRUE>:
# DISTFILES = "media_qtro.json"
# PLUGIN_EXTENDS = "ifmedia"
