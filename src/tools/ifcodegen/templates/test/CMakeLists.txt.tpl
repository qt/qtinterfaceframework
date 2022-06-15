{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/tst_{{interface|lower}}.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::InterfaceFramework
    Qt6::Test
)

if (TARGET ${CURRENT_TARGET})
    target_sources(${CURRENT_TARGET}
                   PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    target_include_directories(${CURRENT_TARGET} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

    target_link_libraries(${CURRENT_TARGET} PUBLIC
        ${${VAR_PREFIX}_LIBRARIES}
    )
endif()
