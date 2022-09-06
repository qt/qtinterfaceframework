{#
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{
    "interfaces" : [
{% for interface in module.interfaces %}
      "{{interface.qualified_name}}"{% if not loop.last %},{%endif%}
{% endfor%}
    ]
}
