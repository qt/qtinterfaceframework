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

if (NOT TARGET Qt6::RemoteObjects OR NOT TARGET Qt6::IfRemoteObjectsHelperPrivate)
    find_package(Qt6 COMPONENTS RemoteObjects IfRemoteObjectsHelperPrivate)
endif()

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/core.h
    ${CMAKE_CURRENT_LIST_DIR}/core.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_REPLICAS
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}.rep
{% endfor %}
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::RemoteObjects
    Qt6::IfRemoteObjectsHelperPrivate
)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    qt6_add_repc_sources(${CURRENT_TARGET}
        ${${VAR_PREFIX}_REPLICAS}
    )

    target_link_libraries(${CURRENT_TARGET} PRIVATE
        ${${VAR_PREFIX}_LIBRARIES}
    )

    target_include_directories(${CURRENT_TARGET} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )
endif()
