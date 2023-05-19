{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.qml.tpl" %}

import QtQuick
import {{module|qml_type}}.simulation

{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}

{{interface|upperfirst}}Backend {
    id: backend

    property QtObject d: QtObject {
        id: d
        property var settings: IfSimulator.findData(IfSimulator.simulationData, "{{interface}}")
        property bool defaultInitialized: false
        property LoggingCategory qLc{{interface|upperfirst}}: LoggingCategory {
            name: "{{module|qml_type|lower}}.simulation.{{interface|lower}}backend"
        }
    }

    function initialize() {
        console.log(d.qLc{{interface|upperfirst}}, "INITIALIZE")
        if (!d.defaultInitialized) {
            IfSimulator.initializeDefault(d.settings, backend)
            d.defaultInitialized = true
        }
        Base.initialize()
    }

{% if interface_zoned %}
    function availableZones() {
        return d.settings.zones;
    }
{% endif %}
{% for property in interface.properties %}

{% if interface_zoned %}
    function {{property|setter_name}}({{property}}, zone) {
        if ("{{property}}" in d.settings && !IfSimulator.checkSettings(d.settings["{{property}}"], {{property}}, zone)) {
            console.error(d.qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IfSimulator.constraint(settings["{{property}}"]));
            return;
        }

        if (zone) {
            console.log(d.qLc{{interface|upperfirst}}, "SIMULATION {{ property }} for zone: " + zone + " changed to: " + {{property}});
            backend.zones[zone].{{property}} = {{property}}
        } else {
            console.log(d.qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
            backend.{{property}} = {{property}}
        }
    }
{% else %}
    function {{property|setter_name}}({{property}}) {
        if ("{{property}}" in d.settings && !IfSimulator.checkSettings(d.settings["{{property}}"], {{property}})) {
            console.error(d.qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IfSimulator.constraint(settings["{{property}}"]));
            return;
        }

        console.log(d.qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
        backend.{{property}} = {{property}}
    }
{% endif %}
{% endfor %}
}
