{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% set class = '{0}Factory'.format(module.module_name|upperfirst) %}
{% set qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
{% set oncedefine = '{0}_H_'.format(class|upper) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}.h>
{% else %}
#include "{{module.module_name|lower}}.h"
{% endif %}
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

{% for struct in module.structs %}
#include "{{struct|lower}}.h"
{% endfor %}

{{ module|begin_namespace }}

class {{exportsymbol}} {{class}} : public {{module.module_name|upperfirst}}
{
    Q_OBJECT
    QML_NAMED_ELEMENT({{qml_name}})
    QML_SINGLETON

public:
    {{class}}(QObject *parent = nullptr);

{% for struct in module.structs %}
    Q_INVOKABLE {{struct}} {{struct|lowerfirst}}() const;
    Q_INVOKABLE {{struct}} {{struct|lowerfirst}}({{struct.fields|map('parameter_type')|join(', ')}}) const;
{% endfor %}
};

{{ module|end_namespace }}

#endif // {{oncedefine}}
