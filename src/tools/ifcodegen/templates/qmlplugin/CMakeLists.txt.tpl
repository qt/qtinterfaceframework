{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:COMM$
##
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## $QT_END_LICENSE$
##
##
##
##
##
##
##
##
#############################################################################
#}
{% include "common/generated_comment.cmake.tpl" %}

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/plugin.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_URI
    {{module|qml_type}}
)

string(REPLACE "." "/" uri_path "{{module|qml_type}}")
qt6_set_ifcodegen_variable(${VAR_PREFIX}_URI_PATH
    ${uri_path}
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_VERSION
    {{module.majorVersion}}.{{module.minorVersion}}
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_TYPEINFO
    ${CMAKE_CURRENT_LIST_DIR}/plugins.qmltypes
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::InterfaceFramework
)

#string(REPLACE "." "/" TARGET_PATH "{{module|qml_type}}")
#qt_add_qml_module(${CURRENT_TARGET}
#    URI "{{module|qml_type}}"
#    VERSION "{{module.majorVersion}}.{{module.minorVersion}}"
#    PLUGIN_TARGET ${CURRENT_TARGET}
#    NO_PLUGIN_OPTIONAL
#    NO_GENERATE_PLUGIN_SOURCE
#    NO_GENERATE_QMLTYPES
#    NO_GENERATE_QMLDIR
#    # TODO remove this again
#    # This is needed to make the test build work as it would create duplicate qmldir entries
#    # in the global qml folder
#    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PATH}
#    SOURCES
#        ${CMAKE_CURRENT_BINARY_DIR}/plugin.cpp
#)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    target_link_libraries(${CURRENT_TARGET}
        PUBLIC
            ${${VAR_PREFIX}_LIBRARIES}
    )

    target_include_directories(${CURRENT_TARGET} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )
endif()

### MISSING
# AUX_QML_FILES += $$PWD/qmldir \
#    $$PWD/designer/{{module.module_name|lower}}.metainfo
