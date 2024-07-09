{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IfRemoteObjectsHelper)
    find_package(Qt6 COMPONENTS RemoteObjects IfRemoteObjectsHelper)
endif()

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}robackend.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}robackend.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}roplugin.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}roplugin.cpp
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
    Qt6::IfRemoteObjectsHelperPrivate
)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    qt6_add_repc_replicas(${CURRENT_TARGET}
        ${${VAR_PREFIX}_REPLICAS}
    )

    target_link_libraries(${CURRENT_TARGET} PRIVATE
        ${${VAR_PREFIX}_LIBRARIES}
    )

    target_include_directories(${CURRENT_TARGET} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )
endif()
