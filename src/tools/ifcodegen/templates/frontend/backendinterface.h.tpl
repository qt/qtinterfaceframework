{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% set class = '{0}BackendInterface'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIfZonedFeatureInterface' %}
{% else %}
{%   set base_class = 'QIfFeatureInterface' %}
{% endif %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if interface.module.tags.config.module %}
#include <{{interface.module.tags.config.module}}/{{module.module_name|lower}}.h>
{% else %}
#include "{{module.module_name|lower}}.h"
{% endif %}

{% for inc in interface|struct_includes %}
{{inc}}
{% endfor %}

#include <QtInterfaceFramework/{{base_class}}>
#include <QtInterfaceFramework/QIfPendingReply>
#include <QtInterfaceFramework/QIfPagingModelInterface>

{{ module|begin_namespace }}

class {{exportsymbol}} {{class}} : public {{base_class}}
{
    Q_OBJECT

public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    virtual {{qtif.prop_setter(property, zoned = interface.tags.config.zoned)}} = 0;
{%   endif %}
{% endfor %}
{% for operation in interface.operations %}
    virtual {{qtif.operation(operation, zoned = interface.tags.config.zoned)}} = 0;
{% endfor %}

Q_SIGNALS:
{% for signal in interface.signals %}
    {{qtif.signal(signal, zoned = interface.tags.config.zoned)}};
{% endfor %}
{% for property in interface.properties %}
    {{qtif.prop_notify(property, zoned = interface.tags.config.zoned, model_interface = true, default_values = true)}};
{% endfor %}
};

#define {{module.module_name|upperfirst}}_{{interface}}_iid ("{{interface.tags.config.id | default(interface.qualified_name)}}")

{{ module|end_namespace }}

#endif // {{oncedefine}}
