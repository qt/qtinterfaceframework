{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}RoPlugin'.format(module.module_name) %}

#include "{{class|lower}}.h"

{% for interface in module.interfaces %}
#include "{{interface|lower}}robackend.h"
{% endfor %}

#include <QStringList>

{{ module|begin_namespace }}

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
{% for interface in module.interfaces %}
    m_interfaces << new {{interface}}RoBackend(QStringLiteral("{{interface.qualified_name}}"), this);
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

{{ module|end_namespace }}
