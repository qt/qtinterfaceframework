{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backend.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backend.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}simulatorplugin.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}simulatorplugin.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::InterfaceFramework
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_PLUGIN_CLASS_NAME
    {{module.module_name|upperfirst}}SimulatorPlugin
)

if (TARGET ${CURRENT_TARGET})
    qt_add_resources(${VAR_PREFIX}_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}_simulation.qrc
    )

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
