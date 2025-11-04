{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% set class = '{0}BackendInterface'.format(interface) %}
{% include 'common/generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

{{ module|begin_namespace }}

/*!
    \class {{class}}
    \inmodule {{module}}
    \ingroup backends
{% if interface.tags.config.zoned %}
    \inherits QIfZonedFeatureInterface
{% else %}
    \inherits QIfFeatureInterface
{% endif %}
    \keyword {{interface.tags.config.id | default(interface.qualified_name)}}

    \brief Backend interface for {{interface}}.

    The {{class}} is the interface used by \l {{interface}}.

    The interface is discovered by a \l {{interface}} object, which connects to it and sets up
    the connections to it.

    \sa {{interface}}
 */
{{class}}::{{class}}(QObject *parent)
{% if interface.tags.config.zoned %}
    : QIfZonedFeatureInterface(parent)
{% else %}
    : QIfFeatureInterface(parent)
{% endif %}
{
}

{{class}}::~{{class}}()
{
}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
/*!
    \fn {{qtif.prop_setter(property, class, interface.tags.config.zoned)}};

    Setter for {{interface}}::{{property}}.
    Sets the property \e {{property}} to the new value passed by \a {{property}}.

{%     if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this property should be changed in.
{%     endif %}

    This method is expected to emit a \l {{property}}Changed() signal when the internal state changes
    due to this function call. The signal is even expected to be emitted if the given \a {{property}} is out of range and no
    actual change takes place.

    \sa {{property}}Changed()
*/
{%   endif %}
{% endfor %}
{% for operation in interface.operations %}
/*!
    \fn {{qtif.operation(operation, class, interface.tags.config.zoned)}};

{{ qtif.format_comments(operation.comment) }}

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this operation should be done in.
{%   endif %}
*/
{% endfor %}

{% for signal in interface.signals %}
/*!
    \fn {{qtif.signal(signal, class, interface.tags.config.zoned)}};

{{ qtif.format_comments(signal.comment) }}

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this operation should be done in.
{%   endif %}
*/
{% endfor %}
{% for property in interface.properties %}
/*!
    \fn {{qtif.prop_notify(property, class, interface.tags.config.zoned)}};

    The signal is emitted when the \e {{property}} property changed to \a {{property}}.

{%   if interface.tags.config.zoned %}
    The value of \a zone indicates the zone this property should be changed in.
{%   endif %}

{%   if not property.readonly and not property.const %}
    \sa {{property|setter_name}}
{%   endif %}
    \sa {{interface}}::{{property}}
*/
{% endfor %}

{{ module|end_namespace }}

#include "moc_{{class|lower}}.cpp"
