# Generated from media.pro.

#####################################################################
## qtifmediaplugin Plugin:
#####################################################################

qt_internal_add_qml_module(qtifmediaplugin
    URI "QtInterfaceFramework.Media"
    VERSION "${PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    CLASS_NAME QIfMediaPlugin
    PLUGIN_TARGET qtifmediaplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    NO_GENERATE_QMLTYPES
    INSTALL_SOURCE_QMLTYPES "plugins.qmltypes"
    SOURCES
        plugin.cpp
    PUBLIC_LIBRARIES
        Qt::InterfaceFramework
        Qt::IfMedia
)

#### Keys ignored in scope 1:.:.:media.pro:<TRUE>:
# CXX_MODULE = "qml"
# OTHER_FILES = "$$qml_files"
# QML_IMPORT_VERSION = "1.0"
# TARGETPATH = "QtInterfaceFramework/Media"
# qml_files = "$$_PRO_FILE_PWD_/qmldir"

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:media.pro:NOT CMAKE_CROSSCOMPILING AND NOT force_independent AND ( NOT debug_and_release OR NOT build_all OR CONFIG(release,debug OR release) ) AND TARGET Qt::Quick:
# QMAKE_EXTRA_TARGETS = "first"
# first.depends = "qmltypes"
# first.target = "first"
