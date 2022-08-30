#####################################################################
## ifmedia-simulation-server app:
#####################################################################

qt_internal_add_app(ifmedia-simulation-server
    SOURCES
        ../../plugins/ifmedia/media_simulator/database_helper.h
        ../../plugins/ifmedia/media_simulator/logging.cpp ../../plugins/ifmedia/media_simulator/logging.h
        ../../plugins/ifmedia/media_simulator/mediadiscoverybackend.cpp ../../plugins/ifmedia/media_simulator/mediadiscoverybackend.h
        ../../plugins/ifmedia/media_simulator/mediaindexerbackend.cpp ../../plugins/ifmedia/media_simulator/mediaindexerbackend.h
        ../../plugins/ifmedia/media_simulator/mediaplayerbackend.cpp ../../plugins/ifmedia/media_simulator/mediaplayerbackend.h
        ../../plugins/ifmedia/media_simulator/searchandbrowsebackend.cpp ../../plugins/ifmedia/media_simulator/searchandbrowsebackend.h
        ../../plugins/ifmedia/media_simulator/usbbrowsebackend.cpp ../../plugins/ifmedia/media_simulator/usbbrowsebackend.h
        ../../plugins/ifmedia/media_simulator/usbdevice.cpp ../../plugins/ifmedia/media_simulator/usbdevice.h
        core.cpp core.h
        main.cpp
        qifmediadiscoveryqtroadapter.cpp qifmediadiscoveryqtroadapter.h
        qifmediaindexerqtroadapter.cpp qifmediaindexerqtroadapter.h
        qifmediaplayerqtroadapter.cpp qifmediaplayerqtroadapter.h
        qiffilterandbrowsemodelqtroadapter.cpp qiffilterandbrowsemodelqtroadapter.h
    INCLUDE_DIRECTORIES
        ../../plugins/ifmedia/media_simulator
    LIBRARIES
        Qt::InterfaceFramework
        Qt::IfMedia
        Qt::Multimedia
        Qt::RemoteObjects
        Qt::Sql
        Qt::IfRemoteObjectsHelperPrivate
)

qt6_add_repc_sources(ifmedia-simulation-server
    qifmediaindexer.rep
    qifmediaplayer.rep
    qifmediadiscoverymodel.rep
    qiffilterandbrowsemodel.rep
)

qt_internal_extend_target(ifmedia-simulation-server CONDITION QT_FEATURE_taglib
    LIBRARIES
        WrapTagLib::WrapTagLib
)
