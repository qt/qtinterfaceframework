# Generated from remoteobjects.pro.

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
    STATIC
    INTERNAL_MODULE
    SOURCES
        qifpagingmodelqtroadapter.cpp qifpagingmodelqtroadapter.h
        qifremoteobjectpendingresult.cpp qifremoteobjectpendingresult.h
        qifremoteobjectreplicahelper.cpp qifremoteobjectreplicahelper.h
        qifremoteobjectsourcehelper.h
    PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::Qml
        Qt::RemoteObjects
)
qt6_add_repc_sources(IfRemoteObjectsHelper
    qifpagingmodel.rep
)
qt6_add_repc_replicas(IfRemoteObjectsHelper
    qifpagingmodel.rep
)

#### Keys ignored in scope 1:.:.:remoteobjects.pro:<TRUE>:
# CMAKE_MODULE_TESTS = "'-'"
# MODULE = "ifremoteobjects_helper"
# targ_headers.CONFIG = "no_check_exist"
