{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
<RCC>
    <qresource prefix="/simulation">
        <file>{{module.module_name|lower}}_simulation_data.json</file>
        <file>{{module.module_name|lower}}_simulation.qml</file>
{% for iface in module.interfaces %}
        <file>{{iface|upperfirst}}Simulation.qml</file>
{% endfor %}
    </qresource>
</RCC>
