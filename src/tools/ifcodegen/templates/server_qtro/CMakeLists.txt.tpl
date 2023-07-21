{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IfRemoteObjectsHelperPrivate)
    find_package(Qt6 COMPONENTS RemoteObjects IfRemoteObjectsHelperPrivate)
endif()

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/core.h
    ${CMAKE_CURRENT_LIST_DIR}/core.cpp
{% if module.tags.config_server_qtro and module.tags.config_server_qtro.useGeneratedMain %}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
{% endif %}
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_REPLICAS
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}.rep
{% endfor %}
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::InterfaceFramework
    Qt6::RemoteObjects
    Qt6::IfRemoteObjectsHelper
)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    qt6_add_repc_sources(${CURRENT_TARGET}
        ${${VAR_PREFIX}_REPLICAS}
    )

    target_include_directories(${CURRENT_TARGET} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

    target_link_libraries(${CURRENT_TARGET} PRIVATE
        ${${VAR_PREFIX}_LIBRARIES}
    )
endif()
