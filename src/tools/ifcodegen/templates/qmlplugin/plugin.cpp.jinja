{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% set exportsymbol = '{0}'.format(module|upper|replace('.', '_')) %}
{% set oncedefine = '{0}QMLPLUGIN_H_'.format(exportsymbol) %}
{% set module_name = module.module_name|upperfirst %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{module.module_name|lower}}factory.h>
{% else %}
#include "{{module.module_name|lower}}factory.h"
{% endif %}

{{ module|begin_namespace }}

class {{module_name}}QmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("{{module|qml_type}}"));
        Q_UNUSED(uri);

        {{module_name}}Factory::registerQmlTypes();
    }
};

{{ module|end_namespace }}

#endif // {{oncedefine}}

#include "plugin.moc"
