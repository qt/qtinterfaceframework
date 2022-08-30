#####################################################################
## IfMedia Module:
#####################################################################

qt_internal_add_qml_module(IfMedia
    URI "QtInterfaceFramework.Media"
    VERSION "${PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    CLASS_NAME QIfMediaPlugin
    SOURCES
        qifamfmtuner.cpp qifamfmtuner.h qifamfmtuner_p.h
        qifamfmtunerbackendinterface.cpp qifamfmtunerbackendinterface.h
        qifmediadevice.cpp qifmediadevice.h
        qifmediadevicediscoverymodel.cpp qifmediadevicediscoverymodel.h qifmediadevicediscoverymodel_p.h
        qifmediadevicediscoverymodelbackendinterface.cpp qifmediadevicediscoverymodelbackendinterface.h
        qifmediaindexercontrol.cpp qifmediaindexercontrol.h qifmediaindexercontrol_p.h
        qifmediaindexercontrolbackendinterface.cpp qifmediaindexercontrolbackendinterface.h
        qifmediaplayer.cpp qifmediaplayer.h qifmediaplayer_p.h
        qifmediaplayerbackendinterface.cpp qifmediaplayerbackendinterface.h
        qifplayableitem.cpp qifplayableitem.h
        qifplayqueue.cpp qifplayqueue.h qifplayqueue_p.h
        qiftunerstation.cpp qiftunerstation.h
        qtifmediaglobal.h qtifmediaglobal_p.h
    IMPORTS
        QtInterfaceFramework/auto
    LIBRARIES
        Qt::CorePrivate
        Qt::InterfaceFrameworkPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::InterfaceFramework
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::InterfaceFrameworkPrivate
)

#####################################################################
## Documentation
#####################################################################

file(GLOB_RECURSE allDocFiles "doc/*.qdoc" "doc/*.png" "doc/*.qdocconf")
add_custom_target(IfMedia_Documentation SOURCES
    ${allDocFiles}
)
