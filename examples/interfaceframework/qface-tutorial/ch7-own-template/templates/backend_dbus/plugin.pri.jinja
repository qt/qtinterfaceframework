{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.qmake.tpl" %}

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}dbusbackend.h \
{% endfor %}
    $$PWD/{{module.module_name|lower}}dbusplugin.h

SOURCES += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}dbusbackend.cpp \
{% endfor %}
    $$PWD/{{module.module_name|lower}}dbusplugin.cpp

{% for interface in module.interfaces %}
{{interface}}.files = {{interface.tags.config_dbus.xml}}
{{interface}}.header_flags += -i dbus_conversion.h

DBUS_INTERFACES += {{interface}}
{% endfor %}
