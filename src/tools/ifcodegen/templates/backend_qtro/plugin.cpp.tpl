{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
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
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}RoPlugin'.format(module.module_name) %}

#include "{{class|lower}}.h"

{% for interface in module.interfaces %}
#include "{{interface|lower}}robackend.h"
{% endfor %}

#include <QStringList>

QT_BEGIN_NAMESPACE

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
{% for interface in module.interfaces %}
    m_interfaces << new {{interface}}RoBackend(QStringLiteral("{{interface.qualified_name}}"), this);
{% endfor %}
}

QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << {{module.module_name|upperfirst}}_{{iface}}_iid{% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

QIfFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
     int index = interfaces().indexOf(interface);
     return index < 0 ? nullptr : m_interfaces.at(index);
}

QT_END_NAMESPACE
