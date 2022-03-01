{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
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

    target_link_libraries(${CURRENT_TARGET} PUBLIC
        ${${VAR_PREFIX}_LIBRARIES}
    )
endif()
