{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}QtRoAdapter'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtIfRemoteObjectsHelper/private/qifremoteobjectssourcehelper_p.h>
#include <QtIfRemoteObjectsHelper/QIfPagingModelSource>

#include "{{interface|lower}}backend.h"
#include "rep_{{interface|lower}}_source.h"

QT_FORWARD_DECLARE_CLASS(QIfPagingModelQtRoAdapter)

{{ module|begin_namespace }}

/*
* When using the templated enableRemoting method of QtRO, there is no way to specify
* the name this object is remoted as.
* This template fixes the name to the interface qualified name
*/
template <class ObjectType>
struct {{interface}}AddressWrapper: public {{interface}}SourceAPI<ObjectType> {
    {{interface}}AddressWrapper(ObjectType *object)
        : {{interface}}SourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};


class {{class}} : public {{interface}}Source
{
    Q_OBJECT
public:
    {{class}}({{interface}}Backend *parent);
    {{class}}(const QString &remoteObjectsLookupName, {{interface}}Backend *parent);

    QString remoteObjectsLookupName() const;
    void enableRemoting(QRemoteObjectHostBase *node);
    void disableRemoting(QRemoteObjectHostBase *node);

{% if interface_zoned %}
    Q_INVOKABLE QStringList availableZones() override;
{% endif %}

{% for property in interface.properties %}
{%   if not property.is_model %}
{%     if interface_zoned %}
    Q_INVOKABLE {{property|return_type}} {{property|getter_name}}(const QString &zone = QString()) override;
{%     else %}
    {{property|return_type}} {{property}}() const override;
{%     endif %}
{%   endif %}
{% endfor %}

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
{%     if interface_zoned %}
    {{qtif.prop_setter(property, zoned = true, default_zone = true)}} override;
{%     else %}
{%       set type = property|return_type %}
{#  //repc doesn't generate proper const ref setters #}
    void {{property|setter_name}}({{type}} {{property}}) override;
{%     endif %}
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    QVariant {{operation}}({{qtif.join_params(operation, zoned = interface_zoned)}}) override;
{% endfor %}

private:
    QString m_remoteObjectsLookupName;
    {{interface}}Backend *m_backend;
    QMultiHash<QRemoteObjectHostBase *, QIfPagingModelQtRoAdapter *> m_modelAdapters;
    QIfRemoteObjectsSourceHelper<{{class}}> m_helper;
};

{{ module|end_namespace }}

#endif // {{oncedefine}}
