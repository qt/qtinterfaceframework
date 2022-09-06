{#
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}DBusBackend'.format(interface) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include "{{interface|lower}}backendinterface.h"
#include "{{interface|lower}}_interface.h"

class {{class}} : public {{interface}}BackendInterface
{
    Q_OBJECT
public:
    {{class}}(QObject *parent = nullptr);

public:
    void initialize() override;

    void setupConnection();
{% for property in interface.properties %}
    void fetch{{property|upperfirst}}();
{% endfor %}
    void checkInitDone();

public Q_SLOTS:
{% for property in interface.properties %}
    void on{{property|upperfirst}}Changed({{property|parameter_type}});
{% endfor %}

private:
    {{interface.tags.config_dbus.className}} *m_client;
    QStringList m_fetchList;
{% for property in interface.properties %}
    {{property|return_type}} m_{{property}};
{% endfor %}
};

#endif // {{oncedefine}}
