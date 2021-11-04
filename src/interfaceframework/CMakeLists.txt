# Generated from interfaceframework.pro.

#####################################################################
## InterfaceFramework Module:
#####################################################################

qt_internal_add_module(InterfaceFramework
    PLUGIN_TYPES interfaceframework
    SOURCES
        qifabstractfeature.cpp qifabstractfeature.h qifabstractfeature_p.h
        qifabstractfeaturelistmodel.cpp qifabstractfeaturelistmodel.h qifabstractfeaturelistmodel_p.h
        qifabstractzonedfeature.cpp qifabstractzonedfeature.h qifabstractzonedfeature_p.h
        qifdefaultpropertyoverrider.cpp qifdefaultpropertyoverrider_p.h
        qiffeatureinterface.cpp qiffeatureinterface.h
        qifpagingmodel.cpp qifpagingmodel.h qifpagingmodel_p.h
        qifpagingmodelinterface.cpp qifpagingmodelinterface.h
        qifpendingreply.cpp qifpendingreply.h qifpendingreply_p.h
        qifproxyserviceobject.cpp qifproxyserviceobject.h qifproxyserviceobject_p.h
        qifqmlconversion_helper.cpp qifqmlconversion_helper.h
        qiffilterandbrowsemodel.cpp qiffilterandbrowsemodel.h qiffilterandbrowsemodel_p.h
        qiffilterandbrowsemodelinterface.cpp qiffilterandbrowsemodelinterface.h
        qifserviceinterface.cpp qifserviceinterface.h
        qifservicemanager.cpp qifservicemanager.h qifservicemanager_p.h
        qifserviceobject.cpp qifserviceobject.h
        qifsimulationengine.cpp qifsimulationengine.h
        qifsimulationglobalobject.cpp qifsimulationglobalobject_p.h
        qifsimulationproxy.cpp qifsimulationproxy.h
        qifstandarditem.cpp qifstandarditem.h
        qifzonedfeatureinterface.cpp qifzonedfeatureinterface.h
        qtinterfaceframeworkmodule.cpp qtinterfaceframeworkmodule.h
        qtifglobal.h qtifglobal_p.h
        queryparser/qifqueryterm.cpp queryparser/qifqueryterm.h queryparser/qifqueryterm_p.h
    DEFINES
        _CRT_NONSTDC_NO_DEPRECATE
    LIBRARIES
        Qt::CorePrivate
        Qt::QmlPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Qml
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
        Qt::QmlPrivate
    EXTRA_CMAKE_FILES
        Qt6InterfaceFrameworkConfigExtras.cmake.in
        Qt6InterfaceFrameworkMacros.cmake
)

# When the system-qface is used we need to save the location of the used Python3 interpreter in
# the module pri file, in order to use from qmake
if(QT_FEATURE_system_qface)
    qt_find_package(Python3 PROVIDED_TARGETS Python3::Interpreter MODULE_NAME interfaceframework)
    set_target_properties(InterfaceFramework PROPERTIES
        QT_MODULE_PRI_EXTRA_CONTENT
"
QMAKE_PYTHON3_LOCATION = ${Python3_EXECUTABLE}
QMAKE_PYTHON3_VERSION = ${Python3_VERSION}
"
    )
endif()

#### Keys ignored in scope 5:.:queryparser:queryparser/queryparser.pri:enable-qlalr:
# QMAKE_EXTRA_COMPILERS = "flex" "qlalr"
# flex.CONFIG = "no_link"
# flex.commands = "flex" "-L" "--nounistd" "-o" "${QMAKE_FILE_OUT}" "${QMAKE_FILE_NAME}"
# flex.dependency_type = "TYPE_C"
# flex.input = "FLEX"
# flex.output = "$$PWD/${QMAKE_FILE_BASE}_flex_p.h"
# qlalr.CONFIG = "no_link"
# qlalr.commands = "cd" "$$PWD;" "$$[QT_INSTALL_BINS]/qlalr" "--qt" "--no-debug" "--no-lines" "--verbose" "${QMAKE_FILE_NAME}"
# qlalr.dependency_type = "TYPE_C"
# qlalr.input = "QLALR"
# qlalr.output = "$$PWD/${QMAKE_FILE_BASE}_p.h"

qt_internal_extend_target(InterfaceFramework CONDITION NOT enable-qlalr
    SOURCES
        queryparser/qifqueryparser_p.h
        queryparser/qifqueryparser_flex_p.h
)

#### Keys ignored in scope 7:.:queryparser:queryparser/queryparser.pri:else:
# OTHER_FILES = "$$FLEX" "$$QLALR"

#####################################################################
## Documentation
#####################################################################

if(QT_BUILD_ONLINE_DOCS)
    set(DOC_CONF "doc/online/qtinterfaceframework.qdocconf")
else()
    set(DOC_CONF "doc/qtinterfaceframework.qdocconf")
endif()

qt_internal_add_docs(InterfaceFramework
    ${DOC_CONF}
)

file(GLOB_RECURSE allDocFiles "doc/*.qdoc" "doc/*.png" "doc/*.qdocconf")
add_custom_target(InterfaceFramework_Documentation SOURCES
    ${allDocFiles}
)
