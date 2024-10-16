{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.qmake.tpl" %}

QT += interfaceframework remoteobjects ifremoteobjects_helper

HEADERS += $$PWD/core.h

SOURCES += $$PWD/core.cpp
{% if module.tags.config_server_qtro and module.tags.config_server_qtro.useGeneratedMain %}
SOURCES += $$PWD/main.cpp
{% endif %}

REPC_SOURCE += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.rep \
{% endfor %}

