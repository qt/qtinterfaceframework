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
{% import 'common/qtif_macros.j2' as qtif %}
{% set class = '{0}'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIfAbstractZonedFeature' %}
{% else %}
{%   set base_class = 'QIfAbstractFeature' %}
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
#include <QtInterfaceFramework/QIfPagingModel>

QT_BEGIN_NAMESPACE

class {{class}}Private;
class {{class}}BackendInterface;

class {{exportsymbol}} {{class}} : public {{base_class}}
{
    Q_OBJECT
    QML_NAMED_ELEMENT({{interface|qml_type}})
{% for property in interface.properties %}
    {{qtif.property(property)}}
{% endfor %}

public:
{% if interface.tags.config.zoned %}
    explicit {{class}}(const QString &zone = QString(), QObject *parent = nullptr);
{% else %}
    explicit {{class}}(QObject *parent = nullptr);
{% endif %}
    ~{{class}}();

    static void registerQmlTypes(const QString& uri, int majorVersion={{interface.module.majorVersion}}, int minorVersion={{interface.module.minorVersion}});

{% for property in interface.properties %}
    {{qtif.prop_getter(property)}};
{% endfor %}

public Q_SLOTS:
{% for operation in interface.operations %}
    {{ qtif.operation(operation) }};
{% endfor %}
{% for property in interface.properties %}
{%   if not property.readonly and not property.const and not property.type.is_model %}
    {{qtif.prop_setter(property)}};
{%   endif %}
{% endfor %}

Q_SIGNALS:
{% for signal in interface.signals %}
    {{qtif.signal(signal)}};
{% endfor %}
{% for property in interface.properties %}
    {{qtif.prop_notify(property)}};
{% endfor %}

protected:
{% if interface.tags.config.zoned %}
    QIfAbstractZonedFeature *createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;
{% endif %}
    {{class}}BackendInterface *{{interface|lower}}Backend() const;

    void connectToServiceObject(QIfServiceObject *service) Q_DECL_OVERRIDE;
    void clearServiceObject() Q_DECL_OVERRIDE;

private:
{% if module.tags.config.disablePrivateIF %}
    friend class {{class}}Private;
    {{class}}Private *m_helper;
{% else %}
{% for property in interface.properties %}
{%   if interface.tags.config.zoned %}
    Q_PRIVATE_SLOT(d_func(), {{qtif.on_prop_changed(property, "", true, true)}})
{%   else %}
    Q_PRIVATE_SLOT(d_func(), {{qtif.on_prop_changed(property, "", false, true)}})
{%   endif %}
{% endfor %}
    Q_DECLARE_PRIVATE({{class}})
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
