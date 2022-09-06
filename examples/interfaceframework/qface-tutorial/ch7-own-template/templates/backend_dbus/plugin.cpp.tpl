{#
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}DBusPlugin'.format(module.module_name) %}

#include "{{class|lower}}.h"

{% for interface in module.interfaces %}
#include "{{interface|lower}}dbusbackend.h"
{% endfor %}

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
{% for interface in module.interfaces %}
    m_interfaces << new {{interface}}DBusBackend(this);
{% endfor %}
}

QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << {{module.module_name|upperfirst}}_{{iface}}_iid{% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

QIfFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
     int index = interfaces().indexOf(interface);
     return index < 0 ? nullptr : m_interfaces.at(index);
}
