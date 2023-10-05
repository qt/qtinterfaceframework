#####################################################################
## IfRemoteObjectsHelper Module:
#####################################################################

# FIXME Find a way to enable this again
# The generated remoteobjects headers are part of this internal module
# This is a problem as the headersclean target checks the headers before
# building the module and at that time the rep headers are not created yet
set(QT_FEATURE_headersclean FALSE)

qt_internal_add_module(IfRemoteObjectsHelper
    CONFIG_MODULE_NAME ifremoteobjects_helper
    SOURCES
        qifpagingmodelqtroadapter.cpp qifpagingmodelqtroadapter_p.h
        qifremoteobjectspendingresult.cpp qifremoteobjectspendingresult_p.h
        qifremoteobjectsreplicahelper.cpp qifremoteobjectsreplicahelper_p.h
        qifremoteobjectssourcehelper_p.h qifremoteobjectshelper.h
        qifremoteobjectshelper.cpp qifremoteobjectshelper.h
    PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::Qml
        Qt::RemoteObjects
)

target_sources(IfRemoteObjectsHelper
  PRIVATE
    qtifremoteobjectshelper_global.h
)
qt6_add_repc_sources(IfRemoteObjectsHelper
    qifpagingmodel.rep
)
qt6_add_repc_replicas(IfRemoteObjectsHelper
    qifpagingmodel.rep
)
