{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
#include <QtTest>
{% for interface in module.interfaces %}
#include "tst_{{interface|lower}}.h"
{% endfor %}

{% set ns = module|namespace %}
{% if ns|length %}
using namespace {{ns}};
{% endif %}

int main(int argc, char *argv[])
{
    int returnCode = 0;
    {% for interface in module.interfaces %}
    {{interface}}Test {{interface|lower}};
    returnCode += QTest::qExec(&{{interface|lower}}, argc, argv);
    {% endfor %}

    return returnCode;
}
