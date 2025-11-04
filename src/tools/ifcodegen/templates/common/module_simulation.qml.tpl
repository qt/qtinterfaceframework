{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.qml.tpl" %}

import QtQuick
import {{module|qml_type}}.simulation

QtObject {
{% for iface in module.interfaces %}
    property var {{iface|lower}} : {{iface|upperfirst}}Simulation {
        id: {{iface|lower}}
    }
{% endfor %}
}
