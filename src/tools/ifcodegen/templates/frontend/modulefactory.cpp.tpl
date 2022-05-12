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
{% set class = '{0}Factory'.format(module.module_name|upperfirst) %}
{% set qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
{% include 'common/generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

QT_BEGIN_NAMESPACE

/*!
    \class {{class}}
    \inmodule {{module}}

    \brief The {{class}} class provides factory methods for all structs defined in the {{module}} module.
*/
/*!
    \qmltype {{qml_name}}
    \instantiates {{class}}
    \inqmlmodule {{module|qml_type}}

    \brief The {{qml_name}} singleton holds all the enums defined in the {{module}} module and
    provides factory methods for all structs.

    The following enums are exported from this object:

{% for enum in module.enums %}
    \section3 {{enum}}
    \include {{module|lower}}module_enum.qdocinc {{enum}}
{% endfor %}
*/
{{class}}::{{class}}(QObject *parent)
    : {{module.module_name|upperfirst}}(parent)
{
}

{% for struct in module.structs %}
/*!
    \qmlmethod {{struct}} {{module|qml_type}}::{{qml_name}}()

    Returns a default instance of {{struct}}

    \sa {{struct}}
*/
/*!
    Returns a default instance of {{struct}}.

    \sa {{struct}}
*/
{{struct}} {{class}}::{{struct|lowerfirst}}() const
{
    return {{struct}}();
}

/*!
    \qmlmethod {{struct}} {{module|qml_type}}::{{qml_name}}({{struct.fields|map('parameter_type')|join(', ')}})

    Returns a default instance of {{struct}}

    \sa {{struct}}
*/
/*!
    Returns a instance of {{struct}} using the passed arguments.

{% for field in struct.fields %}
{%   if field.type.is_enum or field.type.is_flag %}
    Available values for {{field}} are:
    \include {{module|lower}}module_enum.qdocinc {{field.type}}
{%   endif %}

{% endfor %}

    \sa {{struct}}
*/
{{struct}} {{class}}::{{struct|lowerfirst}}({{struct.fields|map('parameter_type')|join(', ')}}) const
{
    return {{struct}}({{struct.fields|join(', ')}});
}

{% endfor %}

QT_END_NAMESPACE
