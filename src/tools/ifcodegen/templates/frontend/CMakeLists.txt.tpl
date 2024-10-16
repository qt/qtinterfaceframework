{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cmake.tpl" %}

qt6_set_ifcodegen_variable(${VAR_PREFIX}_SOURCES
{% for interface in module.interfaces %}
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}_p.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backendinterface.h
    ${CMAKE_CURRENT_LIST_DIR}/{{interface|lower}}backendinterface.cpp
{% endfor %}
{% for struct in module.structs %}
    ${CMAKE_CURRENT_LIST_DIR}/{{struct|lower}}.h
    ${CMAKE_CURRENT_LIST_DIR}/{{struct|lower}}.cpp
{% endfor %}
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}global.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}.cpp
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}factory.h
    ${CMAKE_CURRENT_LIST_DIR}/{{module.module_name|lower}}factory.cpp
)

qt6_set_ifcodegen_variable(${VAR_PREFIX}_DEFINES
    QT_BUILD_{{module|upper|replace('.', '_')|strip_QT}}_LIB
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

qt6_set_ifcodegen_variable(${VAR_PREFIX}_LIBRARIES
    Qt6::InterfaceFramework
{% if not module.tags.config.disablePrivateIF %}
    Qt6::InterfaceFrameworkPrivate
{% endif %}
)

if (TARGET ${CURRENT_TARGET})
    target_compile_definitions(${CURRENT_TARGET} PRIVATE ${${VAR_PREFIX}_DEFINES})

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
