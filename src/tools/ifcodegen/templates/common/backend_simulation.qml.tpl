{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
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
{% include "common/generated_comment.qml.tpl" %}

import QtQuick
import {{module|qml_type}}.simulation

{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}

QtObject {
    property var settings : IfSimulator.findData(IfSimulator.simulationData, "{{interface}}")
    property bool defaultInitialized: false
    property LoggingCategory qLc{{interface|upperfirst}}: LoggingCategory {
        name: "{{module|qml_type|lower}}.simulation.{{interface|lower}}backend"
    }
    property var backend : {{interface|upperfirst}}Backend {

        function initialize() {
            console.log(qLc{{interface|upperfirst}}, "INITIALIZE")
            if (!defaultInitialized) {
                IfSimulator.initializeDefault(settings, backend)
                defaultInitialized = true
            }
            Base.initialize()
        }

{% if interface_zoned %}
        function availableZones() {
            return settings.zones;
        }
{% endif %}
{% for property in interface.properties %}

{% if interface_zoned %}
        function {{property|setter_name}}({{property}}, zone) {
            if ("{{property}}" in settings && !IfSimulator.checkSettings(settings["{{property}}"], {{property}}, zone)) {
                console.error(qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IfSimulator.constraint(settings["{{property}}"]));
                return;
            }

            if (zone) {
                console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} for zone: " + zone + " changed to: " + {{property}});
                backend.zones[zone].{{property}} = {{property}}
            } else {
                console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
                backend.{{property}} = {{property}}
            }
        }
{% else %}
        function {{property|setter_name}}({{property}}) {
            if ("{{property}}" in settings && !IfSimulator.checkSettings(settings["{{property}}"], {{property}})) {
                console.error(qLc{{interface|upperfirst}}, "SIMULATION changing {{property}} is not possible: provided: " + {{property}} + " constraint: " + IfSimulator.constraint(settings["{{property}}"]));
                return;
            }

            console.log(qLc{{interface|upperfirst}}, "SIMULATION {{ property }} changed to: " + {{property}});
            backend.{{property}} = {{property}}
        }
{% endif %}
{% endfor %}
    }
}
