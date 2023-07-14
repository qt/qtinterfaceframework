#####################################################################
## InterfaceFramework Module:
#####################################################################

qt_internal_add_qml_module(InterfaceFramework
    URI "QtInterfaceFramework"
    VERSION "${PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    CLASS_NAME QtInterfaceFrameworkPlugin
    PLUGIN_TYPES interfaceframework
    SOURCES
        qifabstractfeature.cpp qifabstractfeature.h qifabstractfeature_p.h
        qifabstractfeaturelistmodel.cpp qifabstractfeaturelistmodel.h qifabstractfeaturelistmodel_p.h
        qifabstractzonedfeature.cpp qifabstractzonedfeature.h qifabstractzonedfeature_p.h
        qifconfiguration.cpp qifconfiguration.h qifconfiguration_p.h
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

if (FLEX_FOUND AND TARGET ${QT_CMAKE_EXPORT_NAMESPACE}::qlalr AND NOT CROSS_COMILE)
    message("Generating queryparser using flex and qlalr")
    FLEX_TARGET(InterfaceFramework
        queryparser/qifqueryparser.l
        ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparser_flex_p.h
        COMPILE_FLAGS "-L --nounistd"
    )

    # qt_process_qlalr doesn't support merged_output and WORKING_DIRECTORY
    # We just call it manually to update the generated code
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparser_p.h
            ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparser.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparsertable_p.h
            ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparsertable.cpp
        COMMAND ${QT_CMAKE_EXPORT_NAMESPACE}::qlalr --no-debug --qt --no-lines ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparser.g
        DEPENDS ${QT_CMAKE_EXPORT_NAMESPACE}::qlalr
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/queryparser/qifqueryparser.g
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/queryparser
        VERBATIM
    )
else ()
    message("Using pregenerated queryparser files")
endif ()

qt_internal_extend_target(InterfaceFramework
    SOURCES
        queryparser/qifqueryparsertable_p.h
        queryparser/qifqueryparsertable.cpp
        queryparser/qifqueryparser_p.h
        queryparser/qifqueryparser.cpp
        queryparser/qifqueryparser_flex_p.h
)

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
