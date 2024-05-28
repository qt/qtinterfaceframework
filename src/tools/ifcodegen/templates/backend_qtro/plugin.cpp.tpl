{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}RoPlugin'.format(module.module_name|upperfirst) %}

#include "{{class|lower}}.h"

#include <QStringList>

using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
{% for interface in module.interfaces %}
    m_{{interface|lower}}Backend = new {{interface}}RoBackend(u"{{interface.qualified_name}}"_s, this);
{% endfor %}
}

QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << QStringLiteral({{module.module_name|upperfirst}}_{{iface}}_iid){% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

/*! \internal */
QIfFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
{% for interface in module.interfaces %}
{%   if loop.first %}
    if (interface == QStringLiteral({{module.module_name|upperfirst}}_{{interface}}_iid))
{%   else %}
    else if (interface == QStringLiteral({{module.module_name|upperfirst}}_{{interface}}_iid))
{%   endif %}
        return m_{{interface|lower}}Backend;
{% endfor %}

    return nullptr;
}

QString {{class}}::id() const
{
{% if module.tags.config_qtro and module.tags.config_qtro.serviceObjectId %}
{%   set serviceObjectId = module.tags.config_qtro.serviceObjectId %}
{% else %}
{%   set serviceObjectId = "{0}_qtro".format(module.name) %}
{% endif %}
    return u"{{serviceObjectId}}"_s;
}

QString {{class}}::configurationId() const
{
{% if module.tags.config_qtro and module.tags.config_qtro.configurationId %}
{%   set configurationId = module.tags.config_qtro.configurationId %}
{% elif module.tags.config.configurationId %}
{%   set configurationId = module.tags.config.configurationId %}
{% else %}
{%   set configurationId = module.name %}
{% endif %}
    return u"{{configurationId}}"_s;
}

void {{class}}::updateServiceSettings(const QVariantMap &settings)
{
{% for interface in module.interfaces %}
    m_{{interface|lower}}Backend->updateServiceSettings(settings);
{% endfor %}
}

{{ module|end_namespace }}
