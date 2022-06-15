{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.rep.tpl" %}
{% set class = '{0}'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% if interface.module.tags.config.module %}
#include <{{interface.module.tags.config.module}}/{{module.module_name|lower}}.h>
{% else %}
#include "{{module.module_name|lower}}.h"
{% endif %}
{% for inc in interface|struct_includes %}
{{inc}}
{% endfor %}

class {{class}}
{
{% for property in interface.properties %}
{%   set propKeyword = '' %}
{%   if property.readonly %}
{%     set propKeyword = 'READONLY' %}
{%   endif %}
{%   if not property.is_model %}
{%     if interface_zoned %}
    SLOT({{property|return_type|replace(" *", "")}} {{property|getter_name}}(const QString &zone))
{%       if not property.readonly %}
    SLOT({{qtif.prop_setter(property, zoned=true)}})
{%       endif %}
    SIGNAL({{property}}Changed({{property|parameter_type}}, const QString &zone))

{%     else %}
    PROP({{property|return_type|replace(" *", "")}} {{property}} {{propKeyword}})
{%     endif %}
{%   endif %}
{% endfor %}

{% if interface_zoned %}
    SLOT(QStringList availableZones())
{% endif %}

{% for operation in interface.operations %}
    SLOT(QVariant {{operation}}({{qtif.join_params(operation, zoned = interface_zoned)}}))
{% endfor %}

    SIGNAL(pendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value))
{% for signal in interface.signals %}
    SIGNAL({{signal}}({{qtif.join_params(signal, zoned = interface_zoned)}}))
{% endfor %}
};
