{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% set class = '{0}'.format(struct) %}
{% include 'common/generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"

#include <qifqmlconversion_helper.h>

using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

class {{class}}Private : public QSharedData
{
public:
    {{class}}Private()
        : QSharedData()
{% for field in struct.fields %}
        , m_{{field}}({{field|default_type_value}})
{% endfor %}
    {}

    {{class}}Private(const {{class}}Private &other)
        : QSharedData(other)
{% for field in struct.fields %}
        , m_{{field}}(other.m_{{field}})
{% endfor %}
    {}

    {{class}}Private({{struct.fields|map('parameter_type')|join(', ')}})
        : QSharedData()
{% for field in struct.fields %}
        , m_{{field}}({{field}})
{% endfor %}
    {}

{% for field in struct.fields %}
    {{field|return_type}} m_{{field}};
{% endfor %}
};

/*!
    \class {{struct}}
    \inmodule {{module}}
{{ qtif.format_comments(struct.comment) }}
*/

{{class}}::{{class}}()
    : QIfStandardItem()
    , d(new {{class}}Private)
{
    {{module.module_name|upperfirst}}::registerTypes();
}

{{class}}::{{class}}(const {{class}} &rhs)
    : QIfStandardItem(rhs)
    , d(rhs.d)
{
    {{module.module_name|upperfirst}}::registerTypes();
}

{{class}} &{{class}}::operator=(const {{class}} &rhs)
{
    QIfStandardItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

{{class}}::{{class}}({{struct.fields|map('parameter_type')|join(', ')}})
    : QIfStandardItem()
    , d(new {{class}}Private({{struct.fields|join(', ')}}))
{
}

/*! \internal */
{{class}}::~{{class}}()
{
}

QString {{class}}::type() const
{
    return QLatin1String("{{struct|lower}}");
}

{% for field in struct.fields %}

/*!
    \property {{class}}::{{field}}
{{ qtif.format_comments(field.comment) }}
{% if field.const %}
    \note This property is constant and the value will not change once an instance has been created.
{% endif %}
*/
{{qtif.prop_getter(field, class)}}
{
    return d->m_{{field}};
}
{%   if not field.readonly and not field.const %}

{{qtif.prop_setter(field, class)}}
{
    d->m_{{field}} = {{field}};
}
{%   endif %}

{% endfor %}

void {{class}}::fromJSON(const QVariant &variant)
{
    QVariant value = qtif_convertFromJSON(variant);
    // First try to convert the values to a Map or a List
    // This is needed as it could also store a QStringList or a Hash
    if (value.canConvert(QMetaType::fromType<QVariantMap>()))
        value.convert(QMetaType::fromType<QVariantMap>());
    if (value.canConvert(QMetaType::fromType<QVariantList>()))
        value.convert(QMetaType::fromType<QVariantList>());

    if (value.metaType() == QMetaType::fromType<QVariantMap>()) {
        QVariantMap map = value.toMap();
{% for field in struct.fields %}
        if (map.contains(u"{{field}}"_s))
            d->m_{{field}} = map.value(u"{{field}}"_s).value<{{field|return_type}}>();
{% endfor %}
    } else if (value.metaType() == QMetaType::fromType<QVariantList>()) {
        QVariantList values = value.toList();
{% for field in struct.fields %}
        d->m_{{field}} = values.value({{loop.index0}}).value<{{field|return_type}}>();
{% endfor %}
    }
}

bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW
{
    if (left.d == right.d)
        return true;
    return (
{% for field in struct.fields %}
        left.{{field}}() == right.{{field}}() {% if not loop.last %}&&{% endif %}

{% endfor %}
    );
}

bool operator!=(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW
{
    return !(left == right);
}

QDataStream &operator<<(QDataStream &stream, const {{class}} &obj)
{
{% for field in struct.fields %}
    stream << obj.{{field}}();
{% endfor %}
    return stream;
}

QDataStream &operator>>(QDataStream &stream, {{class}} &obj)
{
{% for field in struct.fields %}
    stream >> obj.d->m_{{field}};
{% endfor %}
    return stream;
}

QDebug &operator<<(QDebug &dbg, const {{class}} &obj)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "{{class}}"
        << '('
{% for field in struct.fields %}
        << obj.{{field}}(){% if not loop.last %} << ", "{% endif %}
{% endfor +%}
        << ')';
    return dbg;
}

{{ module|end_namespace }}
