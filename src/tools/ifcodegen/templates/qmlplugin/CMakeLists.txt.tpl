{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

message(WARNING "The qmlplugin template should only be used for porting from qmake to CMake.
Please use one of the alternatives mentioned here:
https://doc.qt.io/QtInterfaceFramework/buildsystem-integration.html#qml-type-registration

E.g the new qt6_ifcodegen_add_qml_module CMake API instead.")

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

qt6_set_ifcodegen_variable(${VAR_PREFIX}_PLUGIN_CLASS_NAME
    {{module.module_name}}QmlPlugin
)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    target_include_directories(${CURRENT_TARGET} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

    target_link_libraries(${CURRENT_TARGET} PRIVATE
        ${${VAR_PREFIX}_LIBRARIES}
    )
endif()
