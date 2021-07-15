{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}Backend'.format(interface) %}
{% set zone_class = '{0}Zone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QObject>
#include <QQmlPropertyMap>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{class}}Interface>
{% else %}
#include "{{class|lower}}interface.h"
{% endif %}

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "common/pagingmodel_simulation.h.tpl" %}
{%   endif %}
{% endfor %}

QT_BEGIN_NAMESPACE

class QIfSimulationEngine;

{% if interface_zoned %}
class {{zone_class}} : public QObject
{
    Q_OBJECT
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     set type = 'QIfPagingModelInterface *' %}
{%   else %}
{%     set type = property|return_type %}
{%   endif %}
    Q_PROPERTY({{type}} {{property}} READ {{property|getter_name}} WRITE {{property|setter_name}}  NOTIFY {{property.name}}Changed FINAL)
{% endfor %}
public:
    explicit {{zone_class}}(const QString &zone, {{class}}Interface *parent = nullptr);

{% for property in interface.properties %}
    {{qtif.prop_getter(property, model_interface = true)}};
{% endfor %}

public Q_SLOTS:
{% for property in interface.properties %}
    {{qtif.prop_setter(property, model_interface = true)}};
{% endfor %}

Q_SIGNALS:
{% for property in interface.properties %}
    {{qtif.prop_notify(property, model_interface = true)}};
{% endfor %}

private:
    {{class}}Interface *m_parent;
    QString m_zone;
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIfPagingModelInterface *m_{{ property }};
{%   else %}
    {{ property|return_type }} m_{{ property }};
{%   endif %}
{% endfor %}
};
{% endif %}

class {{class}} : public {{class}}Interface
{
    Q_OBJECT

{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     set type = 'QIfPagingModelInterface *' %}
{%   else %}
{%     set type = property|return_type %}
{%   endif %}
    Q_PROPERTY({{type}} {{property}} READ {{property|getter_name}} WRITE {{property|setter_name}}  NOTIFY {{property.name}}Changed FINAL)
{% endfor %}
{% if interface_zoned %}
    Q_PROPERTY(QQmlPropertyMap *zones READ zones CONSTANT)
{% endif %}
public:
    explicit {{class}}(QObject *parent = nullptr);
    explicit {{class}}(QIfSimulationEngine *engine, QObject *parent = nullptr);
    ~{{class}}();

{%   if interface_zoned %}
    Q_INVOKABLE QStringList availableZones() const override;
{%   endif %}

    Q_INVOKABLE void initialize() override;
{% if interface_zoned %}
    void addZone(const QString &zone);
    {{zone_class}}* zoneAt(const QString &zone);
{% endif %}

{% for property in interface.properties %}
{%   if interface_zoned %}
{%     if property.type.is_model %}
{%       set type = 'QIfPagingModelInterface *' %}
{%     else %}
{%       set type = property|return_type %}
{%     endif %}
    Q_INVOKABLE {{type}} {{property|getter_name}}(const QString &zone = QString());
{%   else %}
    {{qtif.prop_getter(property, model_interface = true)}};
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    QQmlPropertyMap *zones() const { return m_zones; }
{% endif %}

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    {{qtif.prop_setter(property, zoned = interface_zoned, default_zone = true)}} override;
{%   else %}
    {{qtif.prop_setter(property, zoned = interface_zoned, model_interface = true, default_zone = true)}};
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    {{qtif.operation(operation, zoned = interface_zoned)}} override;
{% endfor %}

protected:
{% for property in interface.properties %}
{#{%   if not property.tags.config_simulator or not property.tags.config_simulator.zoned %}#}
{%     if property.type.is_model %}
    QIfPagingModelInterface *m_{{ property }};
{%     else %}
    {{ property|return_type }} m_{{ property }};
{%     endif %}
{#{%   endif %}#}
{% endfor %}
{% if interface_zoned %}
    QQmlPropertyMap *m_zones;
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
