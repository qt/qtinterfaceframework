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
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backend.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backend.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}simulatorplugin.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}simulatorplugin.cpp
)

if (TARGET ${CURRENT_TARGET})
    qt_add_resources(${VAR_PREFIX}_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}_simulation.qrc
    )

    target_sources(${CURRENT_TARGET}
        PRIVATE
        ${${VAR_PREFIX}_SOURCES}
    )

    target_include_directories(${CURRENT_TARGET} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

    ### MISSING
    # OTHER_FILES += \
    #    $$PWD/{{module.module_name|lower}}.json \
    #    $$PWD/{{module.module_name|lower}}_simulation_data.json
endif()
