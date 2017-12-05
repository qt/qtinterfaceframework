{#
# Copyright (C) 2017 Pelagicore AG
# Contact: https://www.qt.io/licensing/
#
# This file is part of the QtIvi module of the Qt Toolkit.
#
# $QT_BEGIN_LICENSE:LGPL-QTAS$
# Commercial License Usage
# Licensees holding valid commercial Qt Automotive Suite licenses may use
# this file in accordance with the commercial license agreement provided
# with the Software or, alternatively, in accordance with the terms
# contained in a written agreement between you and The Qt Company.  For
# licensing terms and conditions see https://www.qt.io/terms-conditions.
# For further information use the contact form at https://www.qt.io/contact-us.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 3 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL3 included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 2.0 or (at your option) the GNU General
# Public license version 3 or any later version approved by the KDE Free
# Qt Foundation. The licenses are as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-2.0.html and
# https://www.gnu.org/licenses/gpl-3.0.html.
#
# $QT_END_LICENSE$
#
# SPDX-License-Identifier: LGPL-3.0
#}
{% set class = '{0}Replica'.format(interface) %}
{% set properties = interface.properties|rejectattr('is_model')|list %}
#pragma once

#include <QRemoteObjectReplica>

#include "{{module.module_name|lower}}module.h"
{% for inc in interface|struct_includes %}
{{inc}}
{% endfor %}

QT_BEGIN_NAMESPACE

class {{class}} : public QRemoteObjectReplica {
    Q_OBJECT
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_TYPE, "{{interface}}")
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_SIGNATURE, "{{interface.qualified_name|hash}}")

{% for property in properties %}
    Q_PROPERTY({{property|return_type}} {{property}} READ {{property}} {% if not property.const %}NOTIFY {{property}}Changed{% endif %})
{% endfor %}

public:
    {{class}}();
private:
    {{class}}(QRemoteObjectNode *node, const QString &name = QString());
public:
    virtual ~{{class}}();

{% for property in properties %}
    {{property|return_type}} {{property}}() const;
{% endfor %}

public Q_SLOTS:
{% for property in properties %}
{%   if not property.readonly and not property.const %}
    void push{{property|upperfirst}}({{property|parameter_type}});
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
    virtual {{operation|return_type}} {{operation}}({{operation.parameters|map('parameter_type')|join(', ')}});
{% endfor %}

Q_SIGNALS:
{% for property in properties %}
    void {{property}}Changed({{property|return_type}});
{% endfor %}
{% for signal in interface.signals %}
    void {{signal}}({{signal|parameter_type}});
{% endfor %}

private:
    void initialize();
private:
    friend class QT_PREPEND_NAMESPACE(QRemoteObjectNode);
};

QT_END_NAMESPACE


