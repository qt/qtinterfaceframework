

#### Inputs

# input taglib
set(INPUT_taglib "undefined" CACHE STRING "")
set_property(CACHE INPUT_taglib PROPERTY STRINGS undefined no qt system)



#### Libraries

qt_find_package(WrapSystemTagLib PROVIDED_TARGETS WrapSystemTagLib::WrapSystemTagLib MODULE_NAME ifmedia QMAKE_LIB taglib)

#### Tests



#### Features

qt_feature("taglib" PRIVATE
    LABEL "taglib"
    CONDITION (WrapSystemTagLib_FOUND OR QT_FEATURE_shared)
    DISABLE INPUT_taglib STREQUAL 'no'
)
qt_feature_definition("taglib" "QTIF_NO_TAGLIB" NEGATE)
qt_feature("system-taglib" PRIVATE
    LABEL "  Using system taglib"
    CONDITION QT_FEATURE_taglib AND WrapSystemTagLib_FOUND
    ENABLE INPUT_taglib STREQUAL 'system'
    DISABLE INPUT_taglib STREQUAL 'qt'
)
qt_feature("media_simulation_backend" PRIVATE
    LABEL "Mediaplayer Simulation Backend"
    CONDITION TARGET Qt::Multimedia AND TARGET Qt::Sql AND QT_FEATURE_future
)
qt_feature("tuner_simulation_backend" PRIVATE
    LABEL "Tuner Simulation Backend"
    CONDITION TARGET Qt::Multimedia
)
qt_feature("media_qtro_backend" PRIVATE
    LABEL "Mediaplayer Qt Remote Objects Backend"
    CONDITION QT_FEATURE_remoteobjects AND TARGET Qt::Multimedia
)
qt_feature("media_qtro_simulation_server" PRIVATE
    LABEL "Mediaplayer Qt Remote Objects Simulation Server"
    CONDITION QT_FEATURE_remoteobjects AND TARGET Qt::Multimedia AND TARGET Qt::Sql
)
qt_feature("ifmedia" PUBLIC
    LABEL "Qt Interface Framework Media"
    CONDITION QT_FEATURE_interfaceframework
)
qt_configure_add_summary_section(NAME "Qt Interface Framework Media")
qt_configure_add_summary_entry(ARGS "taglib")
qt_configure_add_summary_entry(ARGS "system-taglib")
qt_configure_add_summary_entry(ARGS "media_qtro_simulation_server")
qt_configure_add_summary_section(NAME "Backends")
qt_configure_add_summary_entry(ARGS "media_simulation_backend")
qt_configure_add_summary_entry(ARGS "media_qtro_backend")
qt_configure_add_summary_entry(ARGS "tuner_simulation_backend")
qt_configure_end_summary_section() # end of "Backends" section
qt_configure_end_summary_section() # end of "Qt Interface Framework Media" section
qt_configure_add_summary_entry(
    ARGS "ifmedia"
    CONDITION NOT QT_FEATURE_ifmedia
)
qt_configure_add_report_entry(
    TYPE ERROR
    MESSAGE [[
The taglib submodule was not initialized
Please initialize the submodule
E.g. by running the following command:
    git submodule init && git submodule update

or disable taglib by passing --taglib=no
]]
    CONDITION NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../3rdparty/taglib/taglib/INSTALL.md" AND QT_FEATURE_taglib AND NOT QT_FEATURE_system_taglib
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE [[
Building taglib was disabled!
The Mediaplayer Simulation Backend will only provide a stub and can't scan media files
]]
    CONDITION INPUT_taglib STREQUAL 'no'
)
qt_configure_add_report_entry(
    TYPE WARNING
    MESSAGE [[
Building Mediaplayer Simulation Backend was disabled!
Please make sure the QtMultimedia module is available.
]]
    CONDITION NOT QT_FEATURE_media_simulation_backend AND NOT TARGET Qt::Multimedia
)
