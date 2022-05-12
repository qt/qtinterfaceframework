{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
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
{% set class = '{0}'.format(interface) %}
{% if interface.tags.config.zoned %}
{%   set base_class = 'QIfAbstractZonedFeature' %}
{% else %}
{%   set base_class = 'QIfAbstractFeature' %}
{% endif %}
{% set oncedefine = '{0}_{1}PRIVATE_H_'.format(module.module_name|upper, class|upper) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "{{module.module_name|lower}}.h"

{% if module.tags.config.disablePrivateIF %}
#include <QObject>
{% else %}
#include <QtInterfaceFramework/private/{{base_class|lower}}_p.h>
{% endif %}
#include <QIfPagingModelInterface>

QT_BEGIN_NAMESPACE

class {{class}};

{% if module.tags.config.disablePrivateIF %}
class {{class}}Private : public QObject
{% else %}
class {{class}}Private : public {{base_class}}Private
{% endif %}
{
public:
{% if module.tags.config.disablePrivateIF %}
    {{class}}Private({{class}} *parent);
{% else %}
{%   if interface.tags.config.zoned %}
    {{class}}Private(const QString &interface, const QString &zone, {{class}} *parent);
{%   else %}
    {{class}}Private(const QString &interface, {{class}} *parent);
{%   endif %}
{% endif %}

    static {{class}}Private *get({{class}} *p);
    static const {{class}}Private *get(const {{class}} *p);
    {{class}} *getParent();

    void clearToDefaults();

{% for property in interface.properties %}
    {{qtif.on_prop_changed(property, zoned = interface.tags.config.zoned, model_interface = true)}};
{% endfor %}
{% for signal in interface.signals %}
    void on{{signal|upperfirst}}({{qtif.join_params(signal, zoned = interface.tags.config.zoned)}});
{% endfor %}

{% if not module.tags.config.disablePrivateIF %}
    bool notify(const QByteArray &propertyName, const QVariant &value) override;

    {{class}} * const q_ptr;
{% endif %}
{% for property in interface.properties %}
    {{property|return_type}} m_{{property}};
{% endfor %}

{% if not module.tags.config.disablePrivateIF %}
    Q_DECLARE_PUBLIC({{class}})
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
