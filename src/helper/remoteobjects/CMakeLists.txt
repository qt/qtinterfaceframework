#####################################################################
## IfRemoteObjectsHelper Module:
#####################################################################

# FIXME Find a way to enable this again
# The generated remoteobjects headers are part of this internal module
# This is a problem as the headersclean target checks the headers before
# building the module and at that time the rep headers are not created yet
set(QT_FEATURE_headersclean FALSE)

qt_internal_add_module(IfRemoteObjectsHelperPrivate
    CONFIG_MODULE_NAME ifremoteobjects_helper
    STATIC
    INTERNAL_MODULE
    SOURCES
        qifpagingmodelqtroadapter.cpp qifpagingmodelqtroadapter.h
        qifremoteobjectspendingresult.cpp qifremoteobjectspendingresult.h
        qifremoteobjectsreplicahelper.cpp qifremoteobjectsreplicahelper.h
        qifremoteobjectssourcehelper.h qifremoteobjectshelper.h
    PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::Qml
        Qt::RemoteObjects
)
qt6_add_repc_sources(IfRemoteObjectsHelperPrivate
    qifpagingmodel.rep
)
qt6_add_repc_replicas(IfRemoteObjectsHelperPrivate
    qifpagingmodel.rep
)
