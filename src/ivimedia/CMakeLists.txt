# Generated from ifmedia.pro.

#####################################################################
## IfMedia Module:
#####################################################################

qt_internal_add_module(IfMedia
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

#### Keys ignored in scope 1:.:.:ifmedia.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# OTHER_FILES = "$$PWD/doc/*.qdocconf" "$$PWD/doc/src/*.qdoc"
