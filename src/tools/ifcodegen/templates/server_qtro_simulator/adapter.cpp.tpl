{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}QtRoAdapter'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{interface|lower}}adapter.h"

#include <QtIfRemoteObjectsHelper/private/qifpagingmodelqtroadapter_p.h>

Q_LOGGING_CATEGORY(qLcRO{{interface}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.remoteobjects", QtInfoMsg)

/*
* A simple QtRO Adapter which is derived from the repc generated *Source class
* Every setter/getter and signal is simply forwarded to the original backend
* Every operation is adapted to use the special PendingResult class as a return value
* to inform the client that the return value is not yet ready. Once the QIfPendingReply is ready
* the value is send with the pendingResultAvailable value
*/

using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

{{class}}::{{class}}({{interface}}Backend *parent)
    : {{class}}(u"{{interface.qualified_name}}"_s, parent)
{
}

{{class}}::{{class}}(const QString &remoteObjectsLookupName, {{interface}}Backend *parent)
    : {{interface}}Source(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
    , m_helper(this, qLcRO{{interface}}())
{
{% for property in interface.properties %}
{%   if not property.type.is_model %}
    connect(m_backend, &{{interface}}Backend::{{property.name}}Changed, this, &{{class}}::{{property.name}}Changed);
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
    connect(m_backend, &{{interface}}Backend::{{signal}}, this, &{{class}}::{{signal}});
{% endfor %}
}

QString {{class}}::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

void {{class}}::enableRemoting(QRemoteObjectHostBase *node)
{
    node->enableRemoting<{{interface}}AddressWrapper>(this);
{% set vars = { 'models': False } %}
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     if vars.update({ 'models': True}) %}{% endif %}
    auto {{property|lowerfirst}}Adapter = new QIfPagingModelQtRoAdapter(u"{{interface.qualified_name}}.{{property}}"_s, m_backend->{{property|getter_name}}());
    node->enableRemoting<QIfPagingModelAddressWrapper>({{property|lowerfirst}}Adapter);
    m_modelAdapters.insert(node, {{property|lowerfirst}}Adapter);
{%   endif %}
{% endfor %}
{% if vars.models and interface_zoned %}
    // When this is called the backend should already been initialized
    const QStringList zones = m_backend->availableZones();
    for (const QString &zone : zones) {
{%   for property in interface.properties %}
{%     if property.type.is_model %}
        auto {{property|lowerfirst}}Adapter = new QIfPagingModelQtRoAdapter(u"{{interface.qualified_name}}.{{property}}."_s + zone, m_backend->zoneAt(zone)->{{property|getter_name}}());
        node->enableRemoting<QIfPagingModelAddressWrapper>({{property|lowerfirst}}Adapter);
        m_modelAdapters.insert(node, {{property|lowerfirst}}Adapter);
{%     endif %}
{%   endfor %}
    }
{% endif %}
}

void {{class}}::disableRemoting(QRemoteObjectHostBase *node)
{
    node->disableRemoting(this);
    const auto adapterList = m_modelAdapters.values(node);
    for (QIfPagingModelQtRoAdapter *adapter : adapterList) {
        node->disableRemoting(adapter);
        delete adapter;
    }
    m_modelAdapters.remove(node);
}

{% if interface_zoned %}
QStringList {{class}}::availableZones()
{
    return m_backend->availableZones();
}
{% endif %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
{%     if interface_zoned %}
{{property|return_type}} {{class}}::{{property|getter_name}}(const QString &zone)
{
{%       if property.type.is_var %}
    return m_helper.toRemoteObjectVariant(m_backend->{{property|getter_name}}(zone));
{%       else %}
    return m_backend->{{property|getter_name}}(zone);
{%       endif %}
}
{%     else %}
{{property|return_type}} {{class}}::{{property}}() const
{
{%       if property.type.is_var %}
    return m_helper.toRemoteObjectVariant(m_backend->{{property|getter_name}}());
{%       else %}
    return m_backend->{{property|getter_name}}();
{%       endif %}
}
{%     endif %}
{%   endif %}
{% endfor %}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
{%     if interface_zoned %}
{{qtif.prop_setter(property, class, zoned = true)}}
{%     else %}
{%       set type = property|return_type %}
{#       //repc doesn't generate proper const ref setters #}
void {{class}}::{{property|setter_name}}({{type}} {{property}})
{%     endif %}
{
{%     set parameters = property.name %}
{%     if interface_zoned %}
{%       set parameters = parameters + ', zone' %}
{%     endif%}
    m_backend->{{property|setter_name}}({{parameters}});
}

{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
QVariant {{class}}::{{operation}}({{qtif.join_params(operation, zoned = interface_zoned)}})
{
{%   set function_parameters = operation.parameters|join(', ') %}
{%   if interface_zoned %}
{%     if operation.parameters|length %}
{%       set function_parameters = function_parameters + ', ' %}
{%     endif %}
{%     set function_parameters = function_parameters + 'zone' %}
{%   endif%}
    QIfPendingReplyBase pendingReply = m_backend->{{operation}}({{function_parameters}});
    qCDebug(qLcRO{{interface}}) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}
{% endfor %}

{{ module|end_namespace }}

#include "moc_{{interface|lower}}adapter.cpp"
