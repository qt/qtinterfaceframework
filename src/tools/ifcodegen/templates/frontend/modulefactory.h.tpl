{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:COMM$
##
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## $QT_END_LICENSE$
##
##
##
##
##
##
##
##
#############################################################################
#}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% set class = '{0}Factory'.format(module.module_name|upperfirst) %}
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

QT_BEGIN_NAMESPACE

class {{exportsymbol}} {{class}} : public {{module.module_name|upperfirst}}
{
    Q_OBJECT

public:
    {{class}}(QObject *parent = nullptr);

{% for struct in module.structs %}
    Q_INVOKABLE {{struct}} {{struct|lowerfirst}}() const;
    Q_INVOKABLE {{struct}} {{struct|lowerfirst}}({{struct.fields|map('parameter_type')|join(', ')}}) const;
{% endfor %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
