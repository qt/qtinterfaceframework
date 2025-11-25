{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set testCases = ["InitBackend", "WithoutBackend", "InvalidBackend", "ClearServiceObject", "ChangeFromBackend", "ChangeFromFrontend", "Methods", "Signals", "Models"] %}
#include <QtTest>
{% for interface in module.interfaces %}
#include "tst_{{interface|lower}}.h"
{% endfor %}

{{ module|begin_namespace }}

class {{module.module_name|upperfirst}}Test : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init()
    {
{%   for interface in module.interfaces %}
        m_{{interface|lower}}.init();
{%   endfor %}
    }

{% for testCase in testCases %}
    void test{{testCase}}()
    {
{%   for interface in module.interfaces %}
        m_{{interface|lower}}.test{{testCase}}();
{%   endfor %}
    }

{% endfor %}
private:
{% for interface in module.interfaces %}
    {{interface}}Test m_{{interface|lower}};
{% endfor %}
};

{{ module|end_namespace }}

#include "main.moc"

{% set ns = module|namespace %}
{% if ns|length %}
using namespace {{ns}};
{% endif %}

QTEST_MAIN({{module.module_name|upperfirst}}Test);
