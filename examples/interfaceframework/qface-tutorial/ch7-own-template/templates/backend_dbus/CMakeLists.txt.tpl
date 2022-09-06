{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.cmake.tpl" %}

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}dbusbackend.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}dbusplugin.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::DBus
    Qt6::InterfaceFramework
)

if (TARGET ${CURRENT_TARGET})
{% for interface in module.interfaces %}
    set_source_files_properties({{interface.tags.config_dbus.xml}} PROPERTIES INCLUDE dbus_conversion.h)
    qt_add_dbus_interface(${VAR_PREFIX}_SOURCES
        {{interface.tags.config_dbus.xml}}
        {{interface|lower}}_interface
    )
{% endfor %}

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
