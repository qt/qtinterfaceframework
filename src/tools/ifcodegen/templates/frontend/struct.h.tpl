{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% import 'common/qtif_macros.j2' as qtif %}
{% set class = '{0}'.format(struct) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
{% set exportsymbol = 'Q_{0}_EXPORT'.format(module|upper|replace('.', '_')) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}.h>
{% else %}
#include "{{module.module_name|lower}}.h"
{% endif %}
{% for inc in struct|struct_includes %}
{{inc}}
{% endfor %}
#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QIfStandardItem>

QT_BEGIN_NAMESPACE

class {{class}}Private;

class {{exportsymbol}} {{class}} : public QIfStandardItem
{
    Q_GADGET
{% for field in struct.fields %}
    {{qtif.property(field, notify=false)}}
{% endfor %}

public:
    {{class}}();
    {{class}}(const {{class}} &rhs);
    {{class}} &operator=(const {{class}} &);
    {{class}}({{struct.fields|map('parameter_type')|join(', ')}});
    ~{{class}}();

    QString type() const override;

{% for field in struct.fields %}
{%   if field.name == "id" or field.name == "name" or field.name == "type" or field.name == "data" %}
    {{qtif.prop_getter(field)}} override;
{%   else %}
    {{qtif.prop_getter(field)}};
{%   endif %}
{%   if not field.readonly and not field.const %}
{%     if field.name == "id" or field.name == "data" %}
    {{qtif.prop_setter(field)}} override;
{%     else %}
    {{qtif.prop_setter(field)}};
{%   endif %}
{%   endif %}
{% endfor %}

protected:
    Q_INVOKABLE void fromJSON(const QVariant &variant);

private:
    QSharedDataPointer<{{class}}Private> d;
    friend {{exportsymbol}} bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;
    friend {{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);
};
Q_DECLARE_TYPEINFO({{class}}, Q_MOVABLE_TYPE);

{{exportsymbol}} bool operator==(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;
{{exportsymbol}} bool operator!=(const {{class}} &left, const {{class}} &right) Q_DECL_NOTHROW;

{{exportsymbol}} QDataStream &operator<<(QDataStream &stream, const {{class}} &obj);
{{exportsymbol}} QDataStream &operator>>(QDataStream &stream, {{class}} &obj);

{{exportsymbol}} QDebug &operator<<(QDebug &dbg, const {{class}} &obj);

QT_END_NAMESPACE

Q_DECLARE_METATYPE({{class}})

#endif // {{oncedefine}}
